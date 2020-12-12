#include "GrenadeExplosion.h"

void GrenadeExplosion::setRandomEnemyDiedSoundTime()
{
	this->nextEnemyDiedSoundMaxTime = (rand() % 800 + 200) * 0.001f;
}

void GrenadeExplosion::setSize(float percent)
{
	this->setWorldScale(sf::Vector2f(percent, percent));

	this->setPosition(
		sf::Vector3f(
			this->getPosition2D().x,
			this->getPosition2D().y,
			-(1.0f - percent)
		)
	);
}

GrenadeExplosion::GrenadeExplosion(sf::Vector2f position)
	: aliveTimer(0.0f), enemyDiedSoundTimer(0.0f), nextEnemyDiedSoundMaxTime(0.0f)
{
	// Position
	this->setPosition(position);

	// World scale
	this->setWorldScale(sf::Vector2f(0, 0));

	// Add animation
	sf::IntRect textureRects[2]
	{
		sf::IntRect(96, 0, 64, 64),
		sf::IntRect(160, 0, -64, 64)
	};
	Animation activeAnim(2, textureRects, 0.1f, true);
	this->addAnimation(activeAnim);

	// Sound
	this->enemyDiesSoundPlayer.setVolume(SettingsHandler::getSoundEffectsVolume());
	this->explosionSound.loadFromFile("Resources/Sounds/grenadeExplosion.wav");
	this->enemyDiedSound.loadFromFile("Resources/Sounds/enemyDead.wav");
	this->enemyDiesSoundPlayer.setBuffer(this->enemyDiedSound);

	this->playSound(this->explosionSound);
}

void GrenadeExplosion::update(float deltaTime)
{
	RenderEntity::update(deltaTime);

	this->aliveTimer += deltaTime;
	this->enemyDiedSoundTimer += deltaTime;

	// Play enemy dies sound asynchronized with the explosion sound
	if (this->enemyDiedSoundTimer >= this->nextEnemyDiedSoundMaxTime &&
		MAX_ALIVE_TIME - this->aliveTimer > this->nextEnemyDiedSoundMaxTime)
	{
		this->enemyDiedSoundTimer = 0.0f;
		this->setRandomEnemyDiedSoundTime();

		this->enemyDiesSoundPlayer.play();
	}

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