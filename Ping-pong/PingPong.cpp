#include "PingPong.h"

PingPong::PingPong() {
	for (int i = 0; i < players.size(); i++) {
		player_move[i].first = false;
		player_move[i].second = false;
		players[i].getPanel().setSize(sf::Vector2f(panel_width, panel_height));
		players[i].getPanel().setFillColor(sf::Color::White);
	}
	// center player's panels
	players[0].setPosition(
		sf::Vector2f((screen_width - panel_width) / 2, panel_height));
	players[1].setPosition(
		sf::Vector2f((screen_width - panel_width) / 2, screen_height - panel_height * 2));

	// placing walls and setting it's size
	// left wall
	walls[0].setPosition(0, 0);
	walls[0].setSize(sf::Vector2f(1, screen_height));
	// right wall
	walls[1].setPosition(screen_width - 1, 0);
	walls[1].setSize(sf::Vector2f(1, screen_height));

	// setting up a ball
	ball.getBall().setRadius(screen_width / 20);
	ball.getBall().setFillColor(sf::Color::Green);
	ball.setPosition(sf::Vector2f(screen_width / 2, screen_height / 2));
	ball.setMoving(sf::Vector2f(-100, 0));
}

void PingPong::update(float dt_time) {
	for (int i = 0; i < players.size(); i++) {
		players[i].setMoving(MoveSide::left, player_move[i].first);
		players[i].setMoving(MoveSide::right, player_move[i].second);
	}

	for (int i = 0; i < players.size(); i++) {
		players[i].update(dt_time);
	}
	ball.update(dt_time);
}

void PingPong::checkCollisions() {
	// collisions checked only for the ball
	if (circleVsRectangle(ball.getBall(), walls[0])) {
		std::cout << "collided: wall[0]" << std::endl;
		ball.setMoving(sf::Vector2f(400, 0));
	}
	else if (circleVsRectangle(ball.getBall(), walls[1])) {
		std::cout << "collided: wall[1]" << std::endl;
		ball.setMoving(sf::Vector2f(-400, 0));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

void PingPong::Run() {
	std::cout << "game started" << std::endl;
	while (true) {
		clock.restart();
		checkCollisions();
		update(clock.getElapsedTime().asSeconds());
	}
}

void PingPong::setPlayerMove(MoveSide side, bool is_enabled, int id) {
	if (id < 0 && id > 2)
		return;

	switch (side) {
	case MoveSide::left:
		player_move[id].first = is_enabled;
		break;
	case MoveSide::right:
		player_move[id].second = is_enabled;
		break;
	}
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

	// distance between circle center point and nearest point of rectangle
	if (std::sqrt(dist_x * dist_x + dist_y * dist_y) <= circle.getRadius()) {
		return true;
	}
	else {
		return false;
	}
}