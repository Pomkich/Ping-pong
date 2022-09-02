#include "Client.h"

Client::Client() {
    state = ClientState::Menu;

    left = false;
    right = false;
    space = false;

	window.create(sf::VideoMode(screen_width, screen_height), "SFML works!");

    if (font.loadFromFile("tms.ttf")) {
        std::cout << "tms loaded" << std::endl;
    }

    for (int i = 0; i < buttons.size(); i++) {
        button_labels[i].setFont(font);
        button_labels[i].setCharacterSize(48);
        button_labels[i].setFillColor(sf::Color::Black);

        buttons[i].setSize(sf::Vector2f(button_width, button_height));
        buttons[i].setFillColor(sf::Color::Green);
        buttons[i].setPosition(sf::Vector2f((screen_width - button_width) / 2,
            offset_top + offset_button * i + button_height * i));

        button_labels[i].setPosition(sf::Vector2f((screen_width - button_width) / 2,
            offset_top + offset_button * i + button_height * i));
    }

    button_labels[0].setString("     Connect");
    button_labels[1].setString(" Create Server");
    button_labels[2].setString("         Exit");


    ip_text.setFont(font);
    ip_text.setPosition(sf::Vector2f((screen_width - button_width) / 2, 300));
    ip_text.setCharacterSize(36);
    ip_text.setFillColor(sf::Color::Black);

    ip_field.setSize(sf::Vector2f(button_width, button_height));
    ip_field.setPosition(sf::Vector2f((screen_width - button_width) / 2, 300));
    ip_field.setFillColor(sf::Color::White);
    ip_field.setOutlineColor(sf::Color::Green);
    ip_field.setOutlineThickness(5);

    ip_str.clear();


	// setting up game graphics
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

void Client::Run() {
    connection = std::make_shared<NetClient>(shared_from_this());
    server = std::make_shared<Server>();

    while (window.isOpen()) {
        HandleInput();
        Render();
        std::this_thread::sleep_for(std::chrono::milliseconds(tick_rate));
    }
}

void Client::Render() {
    window.clear();

    switch (state) {
    case ClientState::Game:
        RenderGame();
        break;

    case ClientState::Menu:
        RenderMenu();
        break;

    case ClientState::EnterIp:
        RenderEnterIp();
        break;
    }

    window.display();
}

void Client::RenderGame() {
    window.draw(player);
    window.draw(enemy);
    window.draw(ball);
    window.draw(walls[0]);
    window.draw(walls[1]);
    window.draw(lose_bounds[0]);
    window.draw(lose_bounds[1]);
}

void Client::RenderMenu() {
    for (auto& button : buttons) {
        window.draw(button);
    }
    for (auto& label : button_labels) {
        window.draw(label);
    }
}

void Client::RenderEnterIp() {
    window.draw(ip_field);
    window.draw(ip_text);
}


void Client::HandleInput() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            connection->Disconnect();
            server->Stop();
            window.close();
        }
        else {
            switch (state) {
            case ClientState::Menu:
                HandleInputMenu(event);
                break;

            case ClientState::Game:
                HandleInputGame(event);
                break;

            case ClientState::EnterIp:
                HandleInputEnterIp(event);
                break;
            }
        }
    }
}

void Client::HandleInputMenu(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Num1:
            state = ClientState::EnterIp;
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
            left = true;
            break;
        case sf::Keyboard::Right:
            right = true;
            break;
        case sf::Keyboard::Space:
            space = true;
            break;
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        case sf::Keyboard::Left:
            left = false;
            break;
        case sf::Keyboard::Right:
            right = false;
            break;
        case sf::Keyboard::Space:
            space = false;
            break;
        }
    }
    input << left << right << space;
    connection->Write(input);
    input.clear();
}

void Client::HandleInputEnterIp(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Num1:
            ip_str += '1';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num2:
            ip_str += '2';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num3:
            ip_str += '3';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num4:
            ip_str += '4';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num5:
            ip_str += '5';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num6:
            ip_str += '6';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num7:
            ip_str += '7';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num8:
            ip_str += '8';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Num9:
            ip_str += '9';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Period:
            ip_str += '.';
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Backspace:
            ip_str.erase(ip_str.end() - 1);
            ip_text.setString(ip_str);
            break;
        case sf::Keyboard::Enter:
            std::cout << "connecting" << std::endl;
            break;
        }
    }
}

void Client::Connect() {
    std::cout << "enter ip please" << std::endl;
    std::string ip;
    std::cin >> ip;
    connection->Connect(ip, game_port);
    state = ClientState::Game;
}

void Client::CreateServer() {
    std::cout << "creating server..." << std::endl;
    server->Run();
    connection->Connect("127.0.0.1", game_port);
    state = ClientState::Game;
}

void Client::Exit() {
    std::cout << "exit" << std::endl;
    connection->Disconnect();
    server->Stop();
    exit(0);
}