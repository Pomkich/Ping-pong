#pragma once
#include "PongObserver.h"
#include <SFML/Graphics.hpp>
#include <array>
#include "Constants.h"
#include "PingPong.h"
#include <thread>

class Client : public PongObserver, public std::enable_shared_from_this<Client> {
private:
	sf::RenderWindow window;
	sf::RectangleShape player;
	sf::RectangleShape enemy;
	sf::CircleShape ball;
	std::shared_ptr<PingPong> ping_pong;

	std::array<sf::RectangleShape, 2> walls;
	std::array<sf::RectangleShape, 2> lose_bounds;

	void Render();
	void HandleInput();

public:
	Client();
	void Run();
	void Initialize();
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) override;
};