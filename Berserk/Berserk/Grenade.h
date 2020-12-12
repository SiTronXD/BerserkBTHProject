#pragma once

#include "RenderEntity.h"
#include "GrenadeExplosion.h"

class Grenade : public RenderEntity
{
private:
	const float START_SPEED = 8.0f;
	const float ALIVE_TIME = 1.0f;

	sf::Vector2f direction;

	GrenadeExplosion* grenadeExplosion;

	sf::SoundBuffer grenadeThrownSound;

	float currentSpeed;
	float aliveTimer;

public:
	Grenade(sf::Vector2f startpos, sf::Vector2f direction);

	void update(float deltaTime) override;

	GrenadeExplosion* getGrenadeExplosion() const;
};