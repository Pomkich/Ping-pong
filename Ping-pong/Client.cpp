#include "Client.h"

Client::Client() {
	window.create(sf::VideoMode(screen_width, screen_height), "SFML works!");

	// setting up graphics
	player.setSize(sf::Vector2f(panel_width, panel_height));
	player.setFillColor(sf::Color::White);
	enemy.setSize(sf::Vector2f(panel_width, panel_height));
	enemy.setFillColor(sf::Color::White);

	// left wall
	walls[0].setPosition(0, 0);
	walls[0].setSize(sf::Vector2f(1, screen_height));
	// right wall
	walls[1].setPosition(screen_width - 1, 0);
	walls[1].setSize(sf::Vector2f(1, screen_height));

	// up bound
	lose_bounds[side_up].setPosition(0, 0);
	lose_bounds[side_up].setSize(sf::Vector2f(screen_width, 5));	// 5 pixels - offset
	lose_bounds[side_up].setFillColor(sf::Color::Red);

	// bottom bound
	lose_bounds[side_bot].setPosition(0, screen_height - 5);
	lose_bounds[side_bot].setSize(sf::Vector2f(screen_width, 5));
	lose_bounds[side_bot].setFillColor(sf::Color::Red);

	ball.setRadius(screen_width / 20);
	ball.setFillColor(sf::Color::Green);
}

void Client::sendCoordinates(int ball_x, int ball_y, int p1_x, int p1_y, int p2_x, int p2_y) {
	ball.setPosition(sf::Vector2f(ball_x, ball_y));
	// in the future need to decide who is who (player_1 enemy or player_2)
	player.setPosition(sf::Vector2f(p1_x, p1_y));
	enemy.setPosition(sf::Vector2f(p2_x, p2_y));
}

void Client::Render() {
    window.clear();

	window.draw(player);
	window.draw(enemy);
	window.draw(ball);
	window.draw(walls[0]);
	window.draw(walls[1]);
	window.draw(lose_bounds[0]);
	window.draw(lose_bounds[1]);

    window.display();
}

void Client::Run() {
	std::thread game_thread = std::thread(&PingPong::Run, &(*ping_pong));
	game_thread.detach();

	while (window.isOpen()) {
		HandleInput();
		Render();
	}
}

void Client::Initialize() {
    ping_pong = std::make_shared<PingPong>(shared_from_this());
}

void Client::HandleInput() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Left:
                ping_pong->notifyKeyPress(PressedKey::left, true, 1);
                break;
            case sf::Keyboard::Right:
                ping_pong->notifyKeyPress(PressedKey::right, true, 1);
                break;
            case sf::Keyboard::A:
                ping_pong->notifyKeyPress(PressedKey::left, true, 0);
                break;
            case sf::Keyboard::D:
                ping_pong->notifyKeyPress(PressedKey::right, true, 0);
                break;
            case sf::Keyboard::Space:
                ping_pong->notifyKeyPress(PressedKey::space, true, 1);
                break;
            case sf::Keyboard::W:
                ping_pong->notifyKeyPress(PressedKey::space, true, 0);
                break;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
            case sf::Keyboard::Left:
                ping_pong->notifyKeyPress(PressedKey::left, false, 1);
                break;
            case sf::Keyboard::Right:
                ping_pong->notifyKeyPress(PressedKey::right, false, 1);
                break;
            case sf::Keyboard::A:
                ping_pong->notifyKeyPress(PressedKey::left, false, 0);
                break;
            case sf::Keyboard::D:
                ping_pong->notifyKeyPress(PressedKey::right, false, 0);
                break;
            case sf::Keyboard::Space:
                ping_pong->notifyKeyPress(PressedKey::space, false, 1);
                break;
            case sf::Keyboard::W:
                ping_pong->notifyKeyPress(PressedKey::space, false, 0);
                break;
            }
        }
    }
}