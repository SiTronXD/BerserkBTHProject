#include <ctime>
#include "Enemy.h"
#include "SMath.h"
#include "SettingsHandler.h"

void Enemy::setNextSoundMaxTime()
{
	this->nextNoiseSoundMaxTime = (rand() % 1000 + 500) * 0.001f;
}

Enemy::Enemy(sf::Vector2f startPosition)
	: lastFramePos(startPosition), walkStep(0.0f, 0.0f), noiseSoundTimer(0.0f), 
	nextNoiseSoundMaxTime(0.0f), lastAttackFrameIndex(0), caughtTime(0.0f),
	dead(false), doDamage(false), canMove(true)
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
	Animation walkAnim(2, walkIntRects, 0.25f, true);
	this->addAnimation(walkAnim);

	// Add attack animation
	sf::IntRect attackIntRects[2]
	{
		sf::IntRect(64, 64 + 32 * this->enemyType, 32, 32),
		sf::IntRect(64 + 32, 64 + 32 * this->enemyType, 32, 32),
	};
	Animation attackAnim(2, attackIntRects, 0.25f, true);
	this->addAnimation(attackAnim);

	// Sounds
	this->deadSound.loadFromFile("Resources/Sounds/enemyDead.wav");
	this->noiseSound.loadFromFile("Resources/Sounds/enemyNoise.wav");

	this->setNextSoundMaxTime();
	this->resetCaughtTime();
}

void Enemy::update(float deltaTime, sf::Vector2f targetPosition)
{
	this->lastFramePos = this->getPosition2D();

	// Get direction
	sf::Vector2f walkDir = targetPosition - this->getPosition2D();
	float walkDirSqrd = SMath::dot(walkDir, walkDir);

	this->noiseSoundTimer += deltaTime;

	// Play noise sound
	if (this->noiseSoundTimer >= this->nextNoiseSoundMaxTime && !this->dead)
	{
		this->noiseSoundTimer = 0.0f;
		this->setNextSoundMaxTime();

		// Lower volume depending on distance to target
		float noiseVolume = SMath::clamp(1.0f - walkDirSqrd / (8 * 8), 0.0f, 1.0f);
		float randomPitch = ((rand() % 100) - 50) * 0.01f + 1.0f;

		this->playSound(this->noiseSound, noiseVolume, randomPitch);
	}

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

void Enemy::kill(bool playDeadSound)
{
	if (!this->dead)
	{
		this->dead = true;

		// Add death animation
		sf::IntRect deadRect(32, 64 + 32 * enemyType, 32, 32);

		// Flip dead rect if the previous rect was flipped
		if (this->getTextureIntRect().left > 16)
		{
			deadRect.left = 64;
			deadRect.width = -32;
		}

		sf::IntRect deadRects[1]{ deadRect };
		Animation deadAnim(1, deadRects, 1.0f, false);
		this->addAnimation(deadAnim);

		// Switch to the latest added animation
		this->setAnimationIndex(this->getNrOfAnimations() - 1);

		// Sound
		if (playDeadSound)
		{
			this->playSound(this->deadSound);
		}
	}
}

void Enemy::caughtInExplosion(float effectTimer, sf::Vector2f explosionPos)
{
	// Record new caught time, even if the enemy has alread died
	if (this->caughtTime < 0.0f)
	{
		this->caughtTime = effectTimer;

		// Save last position
		this->lastPos = this->getPosition2D();
	}

	if (this->caughtTime < 0.6f)
	{
		float relativeEffectTimer = (effectTimer - this->caughtTime) / (1.0f - this->caughtTime);
		this->canMove = false;

		// Kill the enemy when it is inside the explosion
		if (relativeEffectTimer >= 0.5f)
			this->kill(false);

		// Move
		float moveT = std::min(std::sin(relativeEffectTimer * 3.1415) * 2.0f, 1.0);
		this->setPosition(SMath::lerp(this->lastPos, explosionPos, moveT));

		// Set size
		float newScale = 1.0f - moveT;
		this->setWorldScale(sf::Vector2f(newScale, newScale));

		// Move down to the ground
		float newZ = -moveT;
		this->setPosition(sf::Vector3f(this->getPosition2D().x, this->getPosition2D().y, newZ));
	}
}

void Enemy::resetCaughtTime()
{
	this->caughtTime = -1.0f;
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
	return this->doDamage && this->canMove;
}

bool Enemy::isDead() const
{
	return this->dead;
}
