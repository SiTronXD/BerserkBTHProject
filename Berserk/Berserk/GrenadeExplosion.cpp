#include "GrenadeExplosion.h"

void GrenadeExplosion::setSize(float percent)
{
	this->setPosition(
		sf::Vector3f(
			this->getPosition2D().x,
			this->getPosition2D().y,
			-(1.0f - percent)
		)
	);
	this->setWorldScale(sf::Vector2f(percent, percent));
}

GrenadeExplosion::GrenadeExplosion(sf::Vector2f position)
	: aliveTimer(0.0f)
{
	// Position
	this->setPosition(position);

	// World scale
	this->setWorldScale(sf::Vector2f(1, 1));

	// Add animation
	sf::IntRect textureRects[2]
	{
		sf::IntRect(96, 0, 64, 64),
		sf::IntRect(160, 0, -64, 64)
	};
	this->addAnimation(Animation(2, textureRects, 0.1f, true));
}

void GrenadeExplosion::update(float deltaTime)
{
	RenderEntity::update(deltaTime);

	this->aliveTimer += deltaTime;

	// Grow
	if (this->aliveTimer <= MAX_GROW_TIME)
	{
		float percent = 1.0f - (MAX_GROW_TIME - this->aliveTimer) / MAX_GROW_TIME;

		// Set new transform
		this->setSize(percent);
	}
	// Shrink
	else if (this->aliveTimer >= MAX_ALIVE_TIME - MAX_SHRINK_TIME)
	{
		float percent = 1.0f - (this->aliveTimer - (MAX_ALIVE_TIME - MAX_SHRINK_TIME)) / MAX_SHRINK_TIME;

		// Set new transform
		this->setSize(percent);

		// Remove
		if (this->aliveTimer >= MAX_ALIVE_TIME)
			this->flagShouldRemove();
	}
}

float GrenadeExplosion::getKillRangeSqrd() const
{
	return this->KILL_RANGE * this->KILL_RANGE;
}

float GrenadeExplosion::getEffectTimer() const
{
	return this->aliveTimer / this->MAX_ALIVE_TIME;
}
