#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include "PongObserver.h"

class NetClient {
private:
	bool interrupt;	// variable used to interrupt listener thread
	std::mutex interrupt_mutex;	// mutex for safe access to interrupt variable

	sf::TcpSocket socket;
	sf::Packet send_data;
	sf::Packet receive_data;
	std::thread listener;

	std::shared_ptr<PongObserver> observer;

public:
	NetClient(std::shared_ptr<PongObserver> obs);
	bool Connect(sf::IpAddress address, unsigned short port);
	void Disconnect();

	void Write(sf::Packet packet);
	void Read();
	void OnRead();
};