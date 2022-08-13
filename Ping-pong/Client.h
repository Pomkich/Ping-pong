#pragma once
#include "PongObserver.h"
#include <SFML/Graphics.hpp>
#include <array>
#include "Constants.h"

class Client : public PongObserver {
private:
	sf::RenderWindow window;
	sf::RectangleShape player;
	sf::RectangleShape enemy;
	sf::CircleShape ball;

	std::array<sf::RectangleShape, 2> walls;
	std::array<sf::RectangleShape, 2> lose_bounds;

	void Render();
	void HandleInput();
	void Run();

public:
	Client();
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) override;
};