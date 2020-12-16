#pragma once

#include "RenderEntity.h"
#include "GrenadeExplosion.h"

class Grenade : public RenderEntity
{
private:
	const float START_SPEED = 8.0f;
	const float ALIVE_TIME = 0.7f;

	sf::Vector2f direction;
	sf::Vector2f lastFramePos;
	sf::Vector2f walkStep;

	GrenadeExplosion* grenadeExplosion;

	sf::SoundBuffer grenadeThrownSound;

	float currentSpeed;
	float aliveTimer;

public:
	Grenade(sf::Vector2f startpos, sf::Vector2f direction);

	void update(float deltaTime) override;

	GrenadeExplosion* getGrenadeExplosion() const;

	sf::Vector2f getLastFramePosition() const;
	sf::Vector2f getWalkStep() const;

	float getCollisionBoxSize() const;
};