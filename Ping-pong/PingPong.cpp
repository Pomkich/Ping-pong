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
	// simulating a work...
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