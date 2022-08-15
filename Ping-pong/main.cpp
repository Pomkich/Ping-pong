#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"
#include <SFML/Network.hpp>
#include "Server.h"

using namespace std;

void connect() {
	char data[8] = "hello w";
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", 57000);
	if (status != sf::Socket::Done)
	{
		//cout << "timeout" << endl;
	}
	else {
		//string address = socket.getRemoteAddress().toString() + '\0';
		//cout << "connected: " << address << endl;
	}
	this_thread::sleep_for(std::chrono::seconds(2));
	while (true) {
		if (socket.send(data, 8) != sf::Socket::Done) {

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

int main() {
	thread thr1(connect);
	thr1.detach();

	thread thr2(connect);
	thr2.detach();

	Server server;
	server.Run();


	/*sf::TcpListener listener;

	// bind the listener to a port
	if (listener.listen(57000) != sf::Socket::Done)
	{
		// error...
	}

	thread connect_thr(connect);
	connect_thr.detach();

	// accept a new connection
	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		cout << "server: timeout" << endl;
	}
	else {
		cout << "server: connection established" << endl;
	}

	char data[12];
	size_t received;

	if (client.receive(data, 12, received) != sf::Socket::Done) {
		cout << "error while data receiving" << endl;
	}
	else {
		cout << "server: data received, " << received << " byres." << endl;
		cout << "received message: " << data;
	}*/



	//std::shared_ptr<Client> clt = std::make_shared<Client>();
	//clt->Initialize();
	//clt->Run();

	return 0;
}