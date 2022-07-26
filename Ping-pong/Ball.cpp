#include "Ball.h"

Ball::Ball() {

}

void Ball::update(float dt_time) {
	sf::Vector2f pos;
	pos.x = ball.getPosition().x + moving_vector.x * dt_time;
	pos.y = ball.getPosition().y + moving_vector.y * dt_time;
	ball.setPosition(pos);
}

sf::CircleShape& Ball::getBall() {
	return ball;
}

void Ball::setMoving(sf::Vector2f moving) {
	moving_vector = moving;
}

void Ball::setPosition(sf::Vector2f pos) {
	ball.setPosition(pos);
}

sf::Vector2f Ball::getMoving() {
	return moving_vector;
}