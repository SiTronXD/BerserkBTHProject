#include "Goal.h"

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