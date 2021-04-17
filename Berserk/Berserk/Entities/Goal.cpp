#include "Goal.h"

const float Goal::GOAL_RADIUS = 2.0f;

Goal::Goal()
	: position(0, 0)
{

}

void Goal::setPosition(sf::Vector2f newPosition)
{
	this->position = newPosition;
}

sf::Vector2f Goal::getPosition() const
{
	return this->position;
}

float Goal::getRadiusSqrd() const
{
	return GOAL_RADIUS * GOAL_RADIUS;
}
