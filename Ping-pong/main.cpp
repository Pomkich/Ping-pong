#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>

using namespace std;

int main() {
	PingPong pong;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(pong.players[0].getPanel());
        window.draw(pong.players[1].getPanel());
        window.draw(pong.ball.getBall());
        window.display();
        this_thread::sleep_for(std::chrono::milliseconds(16));
    }

	return 0;
}