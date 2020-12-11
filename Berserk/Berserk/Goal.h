#pragma once

#include <SFML/Graphics.hpp>

class Goal
{
private:
	static const float GOAL_RADIUS;

	sf::Vector2f position;

	Goal(const Goal& other) = delete;
	Goal operator=(const Goal& other) = delete;

public:
	Goal();

	void setPosition(sf::Vector2f newPosition);

	sf::Vector2f getPosition() const;
	float getRadiusSqrd() const;
};