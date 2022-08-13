#include "Client.h"

Client::Client() {
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
	window.draw(player);
	window.draw(enemy);
	window.draw(ball);
}

void Client::HandleInput() {
	
}