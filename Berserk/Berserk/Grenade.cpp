#include "Grenade.h"
#include "SettingsHandler.h"

Grenade::Grenade(sf::Vector2f startPos, sf::Vector2f direction)
	: direction(direction), aliveTimer(ALIVE_TIME), grenadeExplosion(nullptr),
	lastFramePos(startPos), walkStep(0.0f, 0.0f)
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
	Animation activeAnim(2, textureRects, 0.1f, true);
	this->addAnimation(activeAnim);

	// Speed
	this->currentSpeed = START_SPEED;

	// Sound
	//this->grenadeThrownSound.loadFromFile("Resources/Sounds/grenadeActivated.wav");
	this->grenadeThrownSound.loadFromFile("Resources/Sounds/grenadeActivated2.wav");

	this->playSound(this->grenadeThrownSound);
}

void Grenade::update(float deltaTime)
{
	RenderEntity::update(deltaTime);

	this->lastFramePos = this->getPosition2D();

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
	sf::Vector3f walkStep3D = newDir * currentSpeed * deltaTime;
	sf::Vector3f newPos = this->getPosition3D() + walkStep3D;
	newPos.z = std::max(newPos.z, -0.95f);	

	// Apply new position
	this->setPosition(newPos);

	// Update walk step
	this->walkStep.x = walkStep3D.x;
	this->walkStep.y = walkStep3D.y;
}

GrenadeExplosion* Grenade::getGrenadeExplosion() const
{
	return this->grenadeExplosion;
}

sf::Vector2f Grenade::getLastFramePosition() const
{
	return this->lastFramePos;
}

sf::Vector2f Grenade::getWalkStep() const
{
	return this->walkStep;
}

float Grenade::getCollisionBoxSize() const
{
	return 0.1f;
}
