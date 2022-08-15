#include "NetClient.h"

NetClient::NetClient() {
	interrupt = false;
	socket.setBlocking(false);
}

void NetClient::Connect(sf::IpAddress address, unsigned short port) {
	socket.setBlocking(true);

	if (socket.connect(address, port) != sf::Socket::Done) {
		std::cout << "timeout" << std::endl;
		return;
	}
	else {
		socket.setBlocking(false);
		std::cout << "connection established" << std::endl;
	}

	socket.setBlocking(false);
	// start reading from socket
	listener = std::move(std::thread(&NetClient::Read, &(*this)));
	listener.detach();
}

void NetClient::Disconnect() {
	{
		std::unique_lock<std::mutex> lock(interrupt_mutex);
		interrupt = true;
	}
	socket.disconnect();
}

void NetClient::Write(sf::Packet packet) {
	send_data = packet;
	if (socket.send(send_data) != sf::Socket::Done) {
		std::cout << "can't send data" << std::endl;
	}
}

void NetClient::Read() {
	while (true) {
		{	// checking whether an interrupt is needed
			std::unique_lock<std::mutex> lock(interrupt_mutex);
			if (interrupt) return;
		}
		if (socket.receive(receive_data) != sf::Socket::Done) {
			//std::cout << "can't receive data" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}

void NetClient::OnRead() {
	std::string message;
	receive_data >> message;
	std::cout << "received message: " << message << std::endl;
}