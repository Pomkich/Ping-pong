#include "Player.h"

Player::Player() {
	
}

void Player::update(float dt_time) {
	sf::Vector2f pos;
	pos.x = panel.getPosition().x + moving_vector.x * dt_time;
	pos.y = panel.getPosition().y + moving_vector.y * dt_time;
	panel.setPosition(pos);
}

sf::RectangleShape& Player::getPanel() {
	return panel;
}

void Player::setMoving(sf::Vector2f moving) {
	moving_vector = moving;
}

void Player::setPosition(sf::Vector2f pos) {
	panel.setPosition(pos);
}