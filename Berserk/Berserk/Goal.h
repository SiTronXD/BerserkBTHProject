#pragma once

#include <SFML/Graphics.hpp>

class Goal
{
private:
	sf::Vector2f position;

public:
	Goal();

	void setPosition(sf::Vector2f newPosition);

	sf::Vector2f getPosition() const;
};