#include "PingPong.h"

PingPong::PingPong() {
	for (int i = 0; i < players.size(); i++) {
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

	// up bound
	lose_bounds[0].setPosition(0, 0);
	lose_bounds[0].setSize(sf::Vector2f(screen_width, 5));	// 5 pixels - offset
	lose_bounds[0].setFillColor(sf::Color::Red);

	// bottom bound
	lose_bounds[1].setPosition(0, screen_height - 5);
	lose_bounds[1].setSize(sf::Vector2f(screen_width, 5));
	lose_bounds[1].setFillColor(sf::Color::Red);

	// setting up a ball
	ball.getBall().setRadius(screen_width / 20);
	ball.getBall().setFillColor(sf::Color::Green);
	ball.setPosition(sf::Vector2f(screen_width / 2, screen_height / 2));
	ball.setMoving(sf::Vector2f(200, -200));
}

void PingPong::update(float dt_time) {
	for (int i = 0; i < players.size(); i++) {
		players[i].update(dt_time);
	}
	ball.update(dt_time);
}

void PingPong::checkCollisions() {
	// collisions checked only for the ball
	// wall collision
	if (circleVsRectangle(ball.getBall(), walls[0]) || 
		circleVsRectangle(ball.getBall(), walls[1])) {
		ball.setMoving(sf::Vector2f(-ball.getMoving().x, ball.getMoving().y));
	}
	else if (circleVsRectangle(ball.getBall(), players[0].getPanel()) ||
		circleVsRectangle(ball.getBall(), players[1].getPanel())) {
		ball.setMoving(sf::Vector2f(ball.getMoving().x, -ball.getMoving().y));

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
		std::cout << "space status: " << is_enabled << std::endl;
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