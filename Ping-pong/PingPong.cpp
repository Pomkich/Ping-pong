#include "PingPong.h"

PingPong::PingPong() {
	for (int i = 0; i < players.size(); i++) {
		player_move[i] = MoveSide::hold;
	}
}

void PingPong::update(float dt_time) {
	for (int i = 0; i < players.size(); i++) {
		switch (player_move[i]) {
		case MoveSide::hold:
			players[i].setMoving(sf::Vector2f(0, 0));
			break;
		case MoveSide::right:
			players[i].setMoving(sf::Vector2f(10, 0));
			break;
		case MoveSide::left:
			players[i].setMoving(sf::Vector2f(-10, 0));
			break;
		}
	}

	for (int i = 0; i < players.size(); i++) {
		players[i].update(dt_time);
	}
	ball.update(dt_time);
}

void PingPong::checkCollisions() {

}

void PingPong::Run() {
	while (true) {
		clock.restart();
		checkCollisions();
		update(clock.getElapsedTime().asSeconds());
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void PingPong::setPlayerMove(MoveSide s, int id) {
	if (id < 0 && id > 2)
		return;

	player_move[id] = s;
}