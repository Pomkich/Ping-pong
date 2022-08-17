#include "Server.h"

Server::Server() {

}

void Server::AcceptConnections() {
	std::unique_ptr<sf::TcpSocket> temp_socket = std::make_unique<sf::TcpSocket>();

	if (listener.listen(57000) != sf::Socket::Done)
		std::cout << "can't listen" << std::endl;
	else std::cout << "starting listen" << std::endl;

	// this thread only accept new connections
	// and rejects them, if lobby is full
	while (true) {
		if (listener.accept(*temp_socket) != sf::Socket::Done) {
			std::cout << "connection error" << std::endl;
		}
		else {
			std::unique_lock<std::mutex> lock(mut);
			// move pointer to any player
			if (player_1 == nullptr) {
				std::cout << "player 1 set" << std::endl;
				player_1 = std::move(temp_socket);
				player_1->setBlocking(false);
			}
			else if (player_2 == nullptr) {
				std::cout << "player 2 set" << std::endl;
				player_2 = std::move(temp_socket);
				player_2->setBlocking(false);
			}
			lock.unlock();

			// if nullptr -> pointer moved to any player
			// need to create new socket for listening
			if (!temp_socket) {
				temp_socket.reset();
				temp_socket = std::make_unique<sf::TcpSocket>();

				// if both player connected -> can start game
				lock.lock();
				if (player_1 != nullptr && player_2 != nullptr)
					OnReady();
				lock.unlock();
			}
			else {	// else lobby is full
				// if it case calls, then game will not run anyway or already running
				sf::Packet temp_packet;
				std::string err_message = "this lobby don't have slots";
				temp_packet << err_message;
				temp_socket->send(temp_packet);
			}
		}
	}
}

void Server::Run() {
	listener_thread = std::move(std::thread(&Server::AcceptConnections, &(*this)));
	listener_thread.detach();

	std::string message;

	while (true) {
		std::unique_lock<std::mutex> lock(mut);

		if (player_1 != nullptr) {
			sf::Socket::Status status = player_1->receive(message_p_1);
			if (status == sf::Socket::Done) {
				message_p_1 >> message;
				std::cout << "message player 1: " << message << std::endl;
			}
			else if (status == sf::Socket::Disconnected) {
				std::cout << "disconnected player 1" << std::endl;
				player_1->disconnect();
				player_1.reset();
			}
			message.clear();
		}
		if (player_2 != nullptr) {
			sf::Socket::Status status = player_2->receive(message_p_2);
			if (status == sf::Socket::Done) {
				message_p_2 >> message;
				std::cout << "message player 2: " << message << std::endl;
			}
			else if (status == sf::Socket::Disconnected) {
				std::cout << "disconnected player 2" << std::endl;
				player_2->disconnect();
				player_2.reset();
			}
			message.clear();
		}
		lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void Server::OnReady() {
	std::cout << "game start" << std::endl;
}