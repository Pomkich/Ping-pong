#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>

using namespace std;

int main() {
	PingPong pong;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");
    thread game_thread = thread(&PingPong::Run, &pong);
    game_thread.detach();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left:
                    pong.setPlayerMove(MoveSide::left, 1);
                    break;
                case sf::Keyboard::Right:
                    pong.setPlayerMove(MoveSide::right, 1);
                    break;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left ||
                    event.key.code == sf::Keyboard::Right) {
                    pong.setPlayerMove(MoveSide::hold, 1);
                }
            }
        }

        window.clear();
        window.draw(pong.players[0].getPanel());
        window.draw(pong.players[1].getPanel());
        window.draw(pong.ball.getBall());
        window.draw(pong.walls[0]);
        window.draw(pong.walls[1]);
        window.display();
        //this_thread::sleep_for(std::chrono::milliseconds(16));
    }

	return 0;
}