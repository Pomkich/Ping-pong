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
	void setMoving(PressedKey side, bool enabled);
	void setPosition(sf::Vector2f pos);

	sf::Vector2f getMoving();
};
