#include "Collectible.h"

Collectible::Collectible(sf::Vector2f position)
{
	this->setPosition(sf::Vector3f(position.x, position.y, 0.0f));
}