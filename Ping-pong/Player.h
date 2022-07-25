#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <iostream>

class Player {
	sf::RectangleShape panel;		// the panel controlled by the player
	sf::Vector2f moving_vector_left;
	sf::Vector2f moving_vector_right;

public:
	Player();
	void update(float dt_time);

	sf::RectangleShape& getPanel();
	void enableMoving(MoveSide side);
	void disableMoving(MoveSide side);
	void setPosition(sf::Vector2f pos);
};
