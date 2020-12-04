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

	// Add attack animation
	sf::IntRect attackIntRects[2]
	{
		sf::IntRect(64, 64 + 32 * this->enemyType, 32, 32),
		sf::IntRect(64 + 32, 64 + 32 * this->enemyType, 32, 32),
	};
	this->addAnimation(Animation(2, attackIntRects, 0.25f, true));
}

void Enemy::update(float deltaTime, sf::Vector2f targetPosition)
{
	// Get direction
	sf::Vector2f walkDir = targetPosition - this->getPosition2D();
	float walkDirSqrd = SMath::dot(walkDir, walkDir);

	// Update animations if the player is within range
	if(walkDirSqrd < this->MAX_PLAYER_VISIBLE_DIST * this->MAX_PLAYER_VISIBLE_DIST)
		RenderEntity::update(deltaTime);


	if (!this->dead)
	{
		// Check if the player is too far away
		if (walkDirSqrd > this->MAX_ATTACK_DIST * this->MAX_ATTACK_DIST)
		{
			// Check if the player is close enough for the enemy to see
			if (walkDirSqrd < this->MAX_PLAYER_VISIBLE_DIST * this->MAX_PLAYER_VISIBLE_DIST)
			{
				SMath::vectorNormalize(walkDir);

				// Move new position
				sf::Vector2f newPosition = this->getPosition2D();
				newPosition += walkDir * MOVEMENT_SPEED * deltaTime;

				// Apply new position
				this->setPosition(newPosition);

				// Set walking animation
				this->setAnimationIndex(0);
			}
		}
		// Enemy should attack the player
		else
		{
			// Switch animation if necessary
			if (this->getCurrentAnimationIndex() != 1)
			{
				this->setAnimationIndex(1);
				this->resetCurrentAnimation();
			}

			// Do damage at the start of the last attack frame
			this->doDamage = false;
			if (this->getCurrentAnimation().getCurrentRectIndex() >= 1)
			{
				if(this->getCurrentAnimation().getCurrentRectIndex() != lastAttackFrameIndex)
					this->doDamage = true;
			}

			this->lastAttackFrameIndex = this->getCurrentAnimation().getCurrentRectIndex();
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

	// Switch to the latest added animation
	this->setAnimationIndex(this->getNrOfAnimations() - 1);
}

bool Enemy::isDoingDamage() const
{
	return this->doDamage;
}

bool Enemy::isDead() const
{
	return this->dead;
}
