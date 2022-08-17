#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

class Server {
private:
	bool ready;

	sf::TcpListener listener;
	std::unique_ptr<sf::TcpSocket> player_1;
	std::unique_ptr<sf::TcpSocket> player_2;
	sf::Packet message_p_1;
	sf::Packet message_p_2;

public:
	Server();
	void Run();

private:
	void AcceptConnections();
};