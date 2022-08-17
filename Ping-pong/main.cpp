#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"
#include <SFML/Network.hpp>
#include "Server.h"
#include "NetClient.h"

using namespace std;

/*void connect() {
	sf::Packet packet;
	std::string message = "hello world asd";
	packet << message;

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
		if (socket.send(packet) != sf::Socket::Done) {

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}*/

int main() {
	NetClient c1;
	std::thread connection_1(&NetClient::Connect, &c1, "127.0.0.1", 57000);
	connection_1.detach();
	//c1.Connect("127.0.0.1", 57000);
	
	NetClient c2;
	std::thread connection_2(&NetClient::Connect, &c2, "127.0.0.1", 57000);
	connection_2.detach();
	//c2.Connect("127.0.0.1", 57000);

	Server server;
	std::thread listen(&Server::Run, &server);
	listen.detach();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	sf::Packet data_packet;
	std::string message = "hello 123";
	data_packet << message;
	c1.Write(data_packet);

	//std::this_thread::sleep_for(std::chrono::seconds(1));

	data_packet.clear();
	message = "bye 321";
	data_packet << message;
	c2.Write(data_packet);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	data_packet.clear();
	message = "-disconnect";
	data_packet << message;
	c1.Write(data_packet);

	data_packet.clear();
	message = "-disconnect";
	data_packet << message;
	c2.Write(data_packet);

	this_thread::sleep_for(std::chrono::seconds(1));

	NetClient c3;
	std::thread connection_3(&NetClient::Connect, &c3, "127.0.0.1", 57000);
	connection_3.detach();

	NetClient c4;
	std::thread connection_4(&NetClient::Connect, &c4, "127.0.0.1", 57000);
	connection_4.detach();


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

	std::this_thread::sleep_for(std::chrono::seconds(30));

	return 0;
}