#include <iostream>
#include <SFML/Graphics.hpp>
#include "PingPong.h"
#include <thread>
#include "Client.h"

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
                    pong.notifyKeyPress(PressedKey::left, true, 1);
                    break;
                case sf::Keyboard::Right:
                    pong.notifyKeyPress(PressedKey::right, true, 1);
                    break;
                case sf::Keyboard::A:
                    pong.notifyKeyPress(PressedKey::left, true, 0);
                    break;
                case sf::Keyboard::D:
                    pong.notifyKeyPress(PressedKey::right, true, 0);
                    break;
                case sf::Keyboard::Space:
                    pong.notifyKeyPress(PressedKey::space, true, 1);
                    break;
                case sf::Keyboard::W:
                    pong.notifyKeyPress(PressedKey::space, true, 0);
                    break;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                case sf::Keyboard::Left:
                    pong.notifyKeyPress(PressedKey::left, false, 1);
                    break;
                case sf::Keyboard::Right:
                    pong.notifyKeyPress(PressedKey::right, false, 1);
                    break;
                case sf::Keyboard::A:
                    pong.notifyKeyPress(PressedKey::left, false, 0);
                    break;
                case sf::Keyboard::D:
                    pong.notifyKeyPress(PressedKey::right, false, 0);
                    break;
                case sf::Keyboard::Space:
                    pong.notifyKeyPress(PressedKey::space, false, 1);
                    break;
                case sf::Keyboard::W:
                    pong.notifyKeyPress(PressedKey::space, false, 0);
                    break;
                }
                
            }
        }

        window.clear();
        window.draw(pong.players[0].getPanel());
        window.draw(pong.players[1].getPanel());
        window.draw(pong.ball.getBall());
        window.draw(pong.walls[0]);
        window.draw(pong.walls[1]);
        window.draw(pong.lose_bounds[0]);
        window.draw(pong.lose_bounds[1]);
        window.display();
        //this_thread::sleep_for(std::chrono::milliseconds(16));
    }

	return 0;
}