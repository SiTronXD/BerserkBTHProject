#include "Collectible.h"

Collectible::Collectible(sf::Vector2f position)
	: collectibleFound(false)
{
	// Set position
	this->setPosition(sf::Vector3f(position.x, position.y, 0.0f));

	// Set animation
	sf::IntRect textureRects[]{ sf::IntRect(0, 0, 64, 64) };
	Animation idleAnim(1, textureRects, 1.0f, false);
	this->addAnimation(idleAnim);

	this->foundSound.loadFromFile("Resources/Sounds/skullKnightFound.wav");
}

void Collectible::found()
{
	if (!this->collectibleFound)
	{
		this->collectibleFound = true;

		// Sound
		this->playSound(this->foundSound);
	}
}

void Collectible::update(float deltaTime)
{
	RenderEntity::update(deltaTime);

	// Shrink if the collectible has been found
	if (this->collectibleFound)
	{
		float newScaleFactor = this->getWorldScale().x - deltaTime * 1.5f;

		// Shrink
		this->setWorldScale(sf::Vector2f(newScaleFactor, 1.0f));

		// Remove
		if (newScaleFactor <= 0.0f)
			this->flagShouldRemove();
	}
}

float Collectible::getRadiusSqrd() const
{
	return COLLISION_RADIUS * COLLISION_RADIUS;
}

bool Collectible::hasBeenFound() const
{
	return this->collectibleFound;
}
