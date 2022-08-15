#include "Server.h"

Server::Server() {
	ready = false;
	memset(message_p_1, 0x0, 32);
	memset(message_p_2, 0x0, 32);
}

void Server::AcceptConnections() {
	bool p_1_connected = false;
	bool p_2_connected = false;

	if (listener.listen(57000) != sf::Socket::Done) {
		std::cout << "can't listen" << std::endl;
	}
	else {
		std::cout << "starting listen..." << std::endl;
	}

	if (listener.accept(player_1) != sf::Socket::Done) {
		std::cout << "player_1 can't connect" << std::endl;
	}
	else {
		std::cout << "player_1 connected" << std::endl;
		p_1_connected = true;
	}
	if (listener.accept(player_2) != sf::Socket::Done) {
		std::cout << "player_2 can't connect" << std::endl;
	}
	else {
		std::cout << "player_2 connected" << std::endl;
		p_2_connected = true;
	}

	if (p_1_connected && p_2_connected) {
		std::cout << "both players connected" << std::endl;
		player_1.setBlocking(false);	// sockets will be checked every 16 milliseconds in cycle
		player_2.setBlocking(false);
		ready = true;
	}
}

void Server::Run() {
	AcceptConnections();

	if (!ready) {
		std::cout << "something went wrong at connection phase" << std::endl;
		return;
	}

	size_t received_1 = 0;
	size_t received_2 = 0;
	while (true) {
		if (player_1.receive(message_p_1, 8, received_1) != sf::Socket::Done) {
			//std::cout << "can't receive from player_1" << std::endl;
		}
		else {
			std::cout << "message player 1: " << message_p_1 << std::endl;
		}
		if (player_2.receive(message_p_2, 8, received_2) != sf::Socket::Done) {
			//std::cout << "can't receive from player_2" << std::endl;
		}
		else {
			std::cout << "message player 2: " << message_p_2 << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}