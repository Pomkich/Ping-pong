#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"

using namespace std;

int main() {
	std::shared_ptr<Client> clt = std::make_shared<Client>();
	clt->Initialize();
	clt->Run();

	return 0;
}