#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <thread>
#include "Ball.h"
#include "Player.h"
#include "Constants.h"
#include <iostream>

class PingPong {
public:
//private:
	std::pair<bool, bool> player_move[2];	// every bool - moving vector is enabled
	std::array<Player, 2> players;
	std::array<sf::RectangleShape, 2> walls;
	Ball ball;
	sf::Clock clock;

public:
	PingPong();
	void Run();

	void setPlayerMove(MoveSide side, bool is_enabled, int id);

private:
	void update(float dt_time);	// warning: if delta time >> 0 so it can't be updated
	void checkCollisions();
	bool circleVsRectangle(sf::CircleShape& circle, sf::RectangleShape& rect);
};