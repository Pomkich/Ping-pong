#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>

using namespace std;

int main() {
	Ball player;
	player.setPosition(sf::Vector2f(100, 100));
	player.setMoving(sf::Vector2f(10, 5));
	sf::Clock clock;
	clock.restart();
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << clock.getElapsedTime().asSeconds() << endl;
	player.update(clock.getElapsedTime().asSeconds());
	cout << player.getBall().getPosition().x << "   " << player.getBall().getPosition().y << endl;

	return 0;
}