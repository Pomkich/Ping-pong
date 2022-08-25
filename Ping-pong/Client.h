#pragma once
#include "PongObserver.h"
#include <SFML/Graphics.hpp>
#include <array>
#include "Constants.h"
#include "PingPong.h"
#include "NetClient.h"
#include "Server.h"
#include <thread>

class Client : public PongObserver, public std::enable_shared_from_this<Client> {
private:
	ClientState state;
	sf::RenderWindow window;

	sf::RectangleShape player;
	sf::RectangleShape enemy;
	sf::CircleShape ball;
	std::array<sf::RectangleShape, 2> walls;
	std::array<sf::RectangleShape, 2> lose_bounds;

	std::shared_ptr<NetClient> connection;
	std::shared_ptr<Server> server;

	// variables needed to send input data to server
	sf::Packet input;
	bool left;
	bool right;
	bool space;

public:
	Client();
	void Run();
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) override;

private:
	void Render();
	void HandleInput();

	void HandleInputMenu(sf::Event& event);
	void HandleInputGame(sf::Event& event);

	void Connect();
	void CreateServer();
	void Exit();
};