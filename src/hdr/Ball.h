#pragma once
#include <SFML/Graphics.hpp>

class Ball {
private:
	sf::CircleShape ball;
	sf::Vector2f moving_vector;

public:
	Ball();
	void update(float dt_time);

	sf::CircleShape& getBall();
	void setMoving(sf::Vector2f moving);
	void setPosition(sf::Vector2f pos);

	sf::Vector2f getMoving();
};
