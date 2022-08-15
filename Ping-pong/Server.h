#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

class Server {
private:
	bool ready;

	sf::TcpListener listener;
	sf::TcpSocket player_1;
	sf::TcpSocket player_2;
	sf::Packet message_p_1;
	sf::Packet message_p_2;

public:
	Server();
	void Run();

private:
	void AcceptConnections();
};