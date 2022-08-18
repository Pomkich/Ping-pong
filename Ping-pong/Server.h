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
	// variables for interrupting threads
	std::mutex interrupt_m_list; bool interrupt_b_list;
	std::mutex interrupt_m_read; bool interrupt_b_read;
	std::mutex interrupt_m_send; bool interrupt_b_send;
	std::condition_variable threads_stoped;

	// variables to access game data
	std::mutex player_access_mut;
	std::mutex send_packet_mut;
	std::condition_variable no_data;

	std::thread listener_thread;
	std::thread read_thread;
	std::thread send_thread;
	
	sf::TcpListener listener;
	std::unique_ptr<sf::TcpSocket> player_1;
	std::unique_ptr<sf::TcpSocket> player_2;
	sf::Packet message_p_1;					// buffers of players messages
	sf::Packet message_p_2;
	std::queue<sf::Packet> data_to_send;	// shared data between consumer and producer
											// SendData - consumer, sendCoordinates - producer

public:
	Server();
	void Run();
	void Stop();

private:
	void AcceptConnections();
	void ReadMessages();
	void SendData();

	void OnReady();
	virtual void sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) override;

	friend class PingPong;
};