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

void Player::enableMoving(MoveSide side) {
	switch (side) {
	case MoveSide::left:
		moving_vector_left.x = -panel_speed;
		break;

	case MoveSide::right:
		moving_vector_right.x = panel_speed;
		break;
	}
}

void Player::disableMoving(MoveSide side) {
	switch (side) {
	case MoveSide::left:
		moving_vector_left.x = 0;
		break;

	case MoveSide::right:
		moving_vector_right.x = 0;
		break;
	}
}

void Player::setPosition(sf::Vector2f pos) {
	panel.setPosition(pos);
}