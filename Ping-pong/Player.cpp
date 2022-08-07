#include "Player.h"

Player::Player() {
	moving_vector_left.x = 0;
	moving_vector_left.y = 0;

	moving_vector_right.x = 0;
	moving_vector_right.y = 0;
}

void Player::update(float dt_time) {
	sf::Vector2f pos;
	pos.x = panel.getPosition().x + (moving_vector_left.x + moving_vector_right.x) * dt_time;
	pos.y = panel.getPosition().y + (moving_vector_left.y + moving_vector_right.y) * dt_time;
	panel.setPosition(pos);
}

sf::RectangleShape& Player::getPanel() {
	return panel;
}

void Player::setMoving(PressedKey side, bool enabled) {
	switch (side) {
	case PressedKey::left:
		moving_vector_left.x = -panel_speed * enabled;	// if !enabled -> equals to zero
		break;											// else to panel speed

	case PressedKey::right:
		moving_vector_right.x = panel_speed * enabled;
		break;
	}
}

void Player::setPosition(sf::Vector2f pos) {
	panel.setPosition(pos);
}

sf::Vector2f Player::getMoving() {
	return moving_vector_left + moving_vector_right;
}