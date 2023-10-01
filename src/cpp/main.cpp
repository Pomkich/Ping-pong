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

	/*NetClient c1;
	std::thread connection_1(&NetClient::Connect, &c1, "127.0.0.1", 57000);
	connection_1.detach();
	//c1.Connect("127.0.0.1", 57000);
	
	NetClient c2;
	std::thread connection_2(&NetClient::Connect, &c2, "127.0.0.1", 57000);
	connection_2.detach();
	//c2.Connect("127.0.0.1", 57000);

	shared_ptr<Server> server = std::make_shared<Server>();
	server->Run();
	std::this_thread::sleep_for(std::chrono::seconds(3));
	server->Stop();*/
	shared_ptr<Client> clt = make_shared<Client>();
	clt->Run();

	std::this_thread::sleep_for(std::chrono::seconds(30));

	return 0;
}