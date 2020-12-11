#include "Grenade.h"
#include "SettingsHandler.h"

Grenade::Grenade(sf::Vector2f startPos, sf::Vector2f direction)
	: direction(direction), aliveTimer(ALIVE_TIME), grenadeExplosion(nullptr)
{
	this->setPosition(startPos);

	// World scale
	this->setWorldScale(sf::Vector2f(0.10f, 0.10f));

	// Add animation
	sf::IntRect textureRects[2]
	{
		sf::IntRect(64, 32, 32, 32),
		sf::IntRect(96, 32, -32, 32)
	};
	this->addAnimation(Animation(2, textureRects, 0.1f, true));

	// Speed
	this->currentSpeed = START_SPEED;

	// Sound
	this->soundPlayer.setVolume(SettingsHandler::getSoundEffectsVolume());
	this->grenadeThrownSound.loadFromFile("Resources/Sounds/grenadeActivated.wav");

	this->soundPlayer.setBuffer(this->grenadeThrownSound);
	this->soundPlayer.play();
}

void Grenade::update(float deltaTime)
{
	RenderEntity::update(deltaTime);

	// Decrease speed
	this->currentSpeed -= deltaTime * (START_SPEED * ALIVE_TIME);
	this->currentSpeed = std::max(this->currentSpeed, 0.0f);

	// Alive timer
	this->aliveTimer -= deltaTime;
	if (this->aliveTimer <= 0.0f)
	{
		// Create grenade explosion
		if (!this->grenadeExplosion)
			this->grenadeExplosion = new GrenadeExplosion(this->getPosition2D());

		// World scale
		float newScale = 0.10f + (aliveTimer * 0.1f * 2.0f);
		this->setWorldScale(sf::Vector2f(newScale, newScale));

		// Too small and should be removed
		if (newScale <= 0.0f)
			this->flagShouldRemove();
	}

	// Move
	sf::Vector3f newDir = sf::Vector3f(
		this->direction.x, 
		this->direction.y, 
		-(1.0f - (this->currentSpeed / START_SPEED))	// Fall down
	);
	sf::Vector3f newPos = this->getPosition3D() + newDir * currentSpeed * deltaTime;
	newPos.z = std::max(newPos.z, -0.95f);	

	this->setPosition(newPos);
}

GrenadeExplosion* Grenade::getGrenadeExplosion() const
{
	return this->grenadeExplosion;
}
