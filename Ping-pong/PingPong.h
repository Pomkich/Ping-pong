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
	GameState game_state;
	std::array<Player, 2> players;
	//std::array<int, 2> player_score;
	std::array<sf::RectangleShape, 2> walls;
	std::array<sf::RectangleShape, 2> lose_bounds;
	Ball ball;
	sf::Clock clock;
	int lost_player_id;	// if player lost in last round then he gets ball

public:
	PingPong();
	void Run();

	void notifyKeyPress(PressedKey key, bool is_enabled, int id);

private:
	void update(float dt_time);	// warning: if delta time >> 0 so it can't be updated
	void checkCollisions();
	bool circleVsRectangle(sf::CircleShape& circle, sf::RectangleShape& rect);
};