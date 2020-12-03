#include <ctime>
#include "Enemy.h"
#include "SMath.h"

Enemy::Enemy(sf::Vector2f startPosition)
	: dead(false)
{
	// Set position
	this->setPosition(startPosition);

	// Set enemy type
	this->enemyType = rand() % 3;

	// Add walking animation
	sf::IntRect walkIntRects[2]
	{
		sf::IntRect(0, 64 + 32 * this->enemyType, 32, 32),
		sf::IntRect(32, 64 + 32 * this->enemyType, -32, 32)
	};
	this->addAnimation(Animation(2, walkIntRects, 0.25f, true));
}

void Enemy::update(float deltaTime, sf::Vector2f targetPosition)
{
	RenderEntity::update(deltaTime);

	if (!this->dead)
	{
		// Get direction
		sf::Vector2f walkDir = targetPosition - this->getPosition2D();

		// Check if the enemy is too far away from the player
		float walkDirSqrd = SMath::dot(walkDir, walkDir);
		if (walkDirSqrd > this->MAX_ATTACK_DIST * this->MAX_ATTACK_DIST && 
			walkDirSqrd < this->MAX_PLAYER_VISIBLE_DIST * this->MAX_PLAYER_VISIBLE_DIST)
		{
			SMath::vectorNormalize(walkDir);

			// Move new position
			sf::Vector2f newPosition = this->getPosition2D();
			newPosition += walkDir * MOVEMENT_SPEED * deltaTime;

			// Apply new position
			this->setPosition(newPosition);
		}
	}
}

void Enemy::kill()
{
	this->dead = true;

	// Add death animation
	sf::IntRect deadRect(32, 64 + 32*enemyType, 32, 32);

	// Flip dead rect if the previous rect was flipped
	if (this->getTextureIntRect().left > 16)
	{
		deadRect.left = 64;
		deadRect.width = -32;
	}

	sf::IntRect deadRects[1]{ deadRect };
	this->addAnimation(Animation(1, deadRects, 1.0f, false));

	// Switch to this new animation
	this->setAnimationIndex(1);
}
