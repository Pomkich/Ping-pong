#include "PingPong.h"

PingPong::PingPong(std::shared_ptr<PongObserver> obs) {
	observer = obs;

	for (int i = 0; i < players.size(); i++) {
		players[i].getPanel().setSize(sf::Vector2f(panel_width, panel_height));
	}
	lost_player_id = side_up;		
	game_state = GameState::prestart;

	// center player's panels
	players[side_bot].setPosition(
		sf::Vector2f((screen_width - panel_width) / 2, screen_height - panel_height * 2));
	players[side_up].setPosition(
		sf::Vector2f((screen_width - panel_width) / 2, panel_height));

	// placing walls and setting it's size
	// left wall
	walls[0].setPosition(0, 0);
	walls[0].setSize(sf::Vector2f(1, screen_height));
	// right wall
	walls[1].setPosition(screen_width - 1, 0);
	walls[1].setSize(sf::Vector2f(1, screen_height));

	// up bound
	lose_bounds[side_up].setPosition(0, 0);
	lose_bounds[side_up].setSize(sf::Vector2f(screen_width, 5));	// 5 pixels - offset

	// bottom bound
	lose_bounds[side_bot].setPosition(0, screen_height - 5);
	lose_bounds[side_bot].setSize(sf::Vector2f(screen_width, 5));

	// setting up a ball
	ball.getBall().setRadius(screen_width / 20);
	ball.setPosition(sf::Vector2f(screen_width / 2, screen_height / 2));
	ball.setMoving(sf::Vector2f(start_ball_speed, -start_ball_speed));
}

void PingPong::Run() {
	interrupt = false;
	game_loop = std::move(std::thread(&PingPong::GameLoop, &(*this)));
	game_loop.detach();
}

void PingPong::Stop() {
	std::unique_lock<std::mutex> lock(interrupt_m);
	interrupt = true;
	// wait until thread will stop
	game_stoped.wait(lock, [&] { return !interrupt; });
}

void PingPong::notifyKeyPress(PressedKey key, bool is_enabled, int id) {
	if (id < 0 && id > 2)
		return;

	switch (key) {
	case PressedKey::left:
		players[id].setMoving(PressedKey::left, is_enabled);
		break;
	case PressedKey::right:
		players[id].setMoving(PressedKey::right, is_enabled);
		break;
	case PressedKey::space:
		if (lost_player_id == id && is_enabled) {
			game_state = GameState::running;
		}
		break;
	}
}

void PingPong::GameLoop() {
	std::cout << "game started" << std::endl;
	auto ball_pos = ball.getBall().getPosition();
	auto player1_pos = players[0].getPanel().getPosition();
	auto player2_pos = players[1].getPanel().getPosition();
	while (true) {
		{	// interruption when needed
			std::unique_lock<std::mutex> lock(interrupt_m);
			if (interrupt) {
				std::cout << "game stoped" << std::endl;
				return;
			}
		}

		clock.restart();
		checkCollisions();
		update(clock.getElapsedTime().asSeconds());

		ball_pos = ball.getBall().getPosition();
		player1_pos = players[0].getPanel().getPosition();
		player2_pos = players[1].getPanel().getPosition();

		observer->sendCoordinates(
			ball_pos.x, ball_pos.y,
			player1_pos.x, player1_pos.y,
			player2_pos.x, player2_pos.y);
	}
}

void PingPong::update(float dt_time) {
	for (int i = 0; i < players.size(); i++) {
		players[i].update(dt_time);
	}

	auto panel = players[lost_player_id].getPanel();	// + 1 to off collision
	double y_offset = (lost_player_id == side_up) ? panel_height + 1 : -ball.getBall().getRadius() * 2 - 1;
	switch (game_state) {
	case GameState::prestart:
		// follow lost player
		ball.setPosition(sf::Vector2f(panel.getPosition().x + panel_width / 4,
			panel.getPosition().y + y_offset));
		break;

	case GameState::running:
		ball.update(dt_time);
		break;
	}
}


void PingPong::checkCollisions() {
	// collisions checked only for the ball
	// wall collision
	if (circleVsRectangle(ball.getBall(), walls[0]) ||
		circleVsRectangle(ball.getBall(), walls[1])) {
		ball.setMoving(sf::Vector2f(-ball.getMoving().x, ball.getMoving().y));
	}
	else if (circleVsRectangle(ball.getBall(), players[side_up].getPanel()) ||
		circleVsRectangle(ball.getBall(), players[side_bot].getPanel())) {
		std::cout << "detected" << std::endl;
		ball.setMoving(sf::Vector2f(ball.getMoving().x, -ball.getMoving().y));
	}
	else if (circleVsRectangle(ball.getBall(), lose_bounds[side_up])) {
		std::cout << "up player lose" << std::endl;
		lost_player_id = side_up;
		game_state = GameState::prestart;
	}
	else if (circleVsRectangle(ball.getBall(), lose_bounds[side_bot])) {
		std::cout << "bottom player lose" << std::endl;
		lost_player_id = side_bot;
		game_state = GameState::prestart;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

bool PingPong::circleVsRectangle(sf::CircleShape& circle, sf::RectangleShape& rect) {
	// finding nearest point to circle of rectangle
	double N_x = std::max(rect.getPosition().x,
		std::min(rect.getPosition().x + rect.getSize().x,
			circle.getPosition().x + circle.getRadius()));

	double N_y = std::max(rect.getPosition().y,
		std::min(rect.getPosition().y + rect.getSize().y,
			circle.getPosition().y + circle.getRadius()));

	double B_x = circle.getPosition().x + circle.getRadius();	// coordinates of center of circle
	double B_y = circle.getPosition().y + circle.getRadius();
	double dist_x = B_x - N_x;	// the result of subtracting two vectors
	double dist_y = B_y - N_y;

	double dist = std::sqrt(dist_x * dist_x + dist_y * dist_y);

	// distance between circle center point and nearest point of rectangle
	if (dist <= circle.getRadius()) {
		double overlap = dist / circle.getRadius();
		// dist_x / overlap - to get original distance
		double offset_x = (dist_x / overlap) * (1 - overlap);
		double offset_y = (dist_y / overlap) * (1 - overlap);
		circle.setPosition(sf::Vector2f(circle.getPosition().x + offset_x, circle.getPosition().y + offset_y));
		return true;
	}
	else {
		return false;
	}
}