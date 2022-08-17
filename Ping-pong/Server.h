#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Server {
private:
	std::mutex mut;
	std::thread listener_thread;

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
	void OnReady();
};