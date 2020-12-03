#include "Collectible.h"

Collectible::Collectible(sf::Vector2f position)
{
	// Set position
	this->setPosition(sf::Vector3f(position.x, position.y, 0.0f));

	// Set animation
	sf::IntRect textureRects[]{ sf::IntRect(0, 0, 64, 64) };
	this->addAnimation(Animation(1, textureRects, 1.0f, false));
}

float Collectible::getRadiusSqrd() const
{
	return COLLISION_RADIUS * COLLISION_RADIUS;
}
