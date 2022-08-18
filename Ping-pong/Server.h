#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "PongObserver.h"
#include <queue>

class Server : public PongObserver {
private:
	std::mutex player_access_mut;
	std::mutex send_packet_mut;
	std::thread listener_thread;
	std::thread read_thread;

	sf::TcpListener listener;
	std::unique_ptr<sf::TcpSocket> player_1;
	std::unique_ptr<sf::TcpSocket> player_2;
	sf::Packet message_p_1;
	sf::Packet message_p_2;
	std::queue<sf::Packet> data_to_send;


public:
	Server();
	void Run();

private:
	void AcceptConnections();
	void ReadMessages();
	//void SendData();

	void OnReady();
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) override;

	friend class PingPong;
};