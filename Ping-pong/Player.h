#pragma once
#include <SFML/Graphics.hpp>

class Player {
	sf::RectangleShape panel;		// the panel controlled by the player
	sf::Vector2f moving_vector;

public:
	Player();
	void update(float dt_time);

	sf::RectangleShape& getPanel();
	void setMoving(sf::Vector2f moving);
	void setPosition(sf::Vector2f pos);
};
