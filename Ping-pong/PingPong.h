#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <thread>
#include <mutex>
#include "Ball.h"
#include "Player.h"
#include "Constants.h"
#include <iostream>
#include "PongObserver.h"

class PingPong {
private:
	GameState game_state;
	std::array<Player, 2> players;
	std::array<sf::RectangleShape, 2> walls;
	std::array<sf::RectangleShape, 2> lose_bounds;
	Ball ball;
	sf::Clock clock;
	int lost_player_id;	// if player lost in last round then he gets ball

	std::thread game_loop;
	std::mutex interrupt_m;
	std::condition_variable game_stoped;
	bool interrupt;

	std::shared_ptr<PongObserver> observer;

public:
	PingPong(std::shared_ptr<PongObserver> obs);
	void Run();
	void Stop();

	void notifyKeyPress(PressedKey key, bool is_enabled, int id);

private:
	void GameLoop();
	void update(float dt_time);	// warning: if delta time >> 0 so it can't be updated
	void checkCollisions();
	bool circleVsRectangle(sf::CircleShape& circle, sf::RectangleShape& rect);
};