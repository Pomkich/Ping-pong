#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"

using namespace std;

int main() {
	Client clt;
	clt.Run();

	return 0;
}