#include <ctime>
#include "Enemy.h"
#include "SMath.h"

Enemy::Enemy(sf::Vector2f startPosition)
	: lastFramePos(startPosition), walkStep(0.0f, 0.0f),
	lastAttackFrameIndex(0), dead(false), doDamage(false), canMove(true)
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
	this->lastFramePos = this->getPosition2D();

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
				if (this->canMove)
				{
					SMath::vectorNormalize(walkDir);

					// Move new position
					this->walkStep = walkDir * MOVEMENT_SPEED * deltaTime;
					sf::Vector2f newPosition = this->getPosition2D() + this->walkStep;

					// Apply new position
					this->setPosition(newPosition);
				}

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

void Enemy::caughtInExplosion(float effectTimer, sf::Vector2f explosionPos)
{
	// Save last position
	if (this->canMove)
		this->lastPos = this->getPosition2D();

	this->canMove = false;

	// Kill the enemy when it is inside the explosion
	if (effectTimer >= 0.5f)
		this->kill();

	// Move
	float moveT = std::min(std::pow(std::sin(effectTimer * 3.1415), 1.0), 1.0) * 2.0f;
	this->setPosition(SMath::lerp(this->lastPos, explosionPos, moveT));

	// Set size
	float newScale = 1.0f - moveT;
	this->setWorldScale(sf::Vector2f(newScale, newScale));

	// Move down to the ground
	float newZ = -moveT;
	this->setPosition(sf::Vector3f(this->getPosition2D().x, this->getPosition2D().y, newZ));
}

sf::Vector2f Enemy::getLastFramePosition() const
{
	return this->lastFramePos;
}

sf::Vector2f Enemy::getWalkStep() const
{
	return this->walkStep;
}

float Enemy::getCollisionBoxSize() const
{
	return 0.4f;
}

bool Enemy::isDoingDamage() const
{
	return this->doDamage;
}

bool Enemy::isDead() const
{
	return this->dead;
}
