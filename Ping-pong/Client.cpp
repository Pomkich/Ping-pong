#include "Client.h"

Client::Client() {
    state = ClientState::Menu;

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
    connection = std::make_shared<NetClient>(shared_from_this());
    server = std::make_shared<Server>();

	while (window.isOpen()) {
		HandleInput();
		Render();
	}
}

void Client::HandleInput() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else {
            switch (state) {
            case ClientState::Menu:
                HandleInputMenu(event);
                break;

            case ClientState::Game:
                HandleInputGame(event);
                break;
            }
        }
    }
}

void Client::HandleInputMenu(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Num1:
            Connect();
            break;

        case sf::Keyboard::Num2:
            CreateServer();
            break;

        case sf::Keyboard::Num3:
            Exit();
            break;
        }
    }
}

void Client::HandleInputGame(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Left:

            break;
        case sf::Keyboard::Right:

            break;

        case sf::Keyboard::Space:

            break;
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        case sf::Keyboard::Left:

            break;
        case sf::Keyboard::Right:

            break;
        case sf::Keyboard::Space:

            break;
        }
    }
}

void Client::Connect() {
    std::cout << "connecting..." << std::endl;
    connection->Connect("127.0.0.1", 57000);
    state = ClientState::Game;
}

void Client::CreateServer() {
    std::cout << "creating server..." << std::endl;
    server->Run();
    connection->Connect("127.0.0.1", 57000);
    state = ClientState::Game;
}

void Client::Exit() {
    std::cout << "exit" << std::endl;
}