#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"
#include <SFML/Network.hpp>
#include "Server.h"
#include "NetClient.h"

using namespace std;

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

	std::this_thread::sleep_for(std::chrono::seconds(1));

	NetClient c3;
	std::thread connection_3(&NetClient::Connect, &c3, "127.0.0.1", 57000);
	connection_3.detach();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	c1.Disconnect();
	c2.Disconnect();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	connection_1 = std::move(std::thread(&NetClient::Connect, &c1, "127.0.0.1", 57000));
	connection_1.detach();

	connection_2 = std::move(std::thread(&NetClient::Connect, &c2, "127.0.0.1", 57000));
	connection_2.detach();

	std::this_thread::sleep_for(std::chrono::seconds(30));

	return 0;
}