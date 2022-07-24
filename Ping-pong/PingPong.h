#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Ball.h"
#include "Player.h"

class PingPong {
private:
	std::array<Player, 2> players;
	std::array<sf::RectangleShape, 2> walls;
	Ball ball;

public:
	PingPong() {

	}
};