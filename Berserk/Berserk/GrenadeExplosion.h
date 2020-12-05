#pragma once

#include "RenderEntity.h"
#include <iostream>

class GrenadeExplosion : public RenderEntity
{
private:
	const float MAX_GROW_TIME = 0.5f;
	const float MAX_SHRINK_TIME = 0.5f;
	const float MAX_ALIVE_TIME = 4.0f;
	const float KILL_RANGE = 5.0f;

	float aliveTimer;

	void setSize(float percent);

public:
	GrenadeExplosion(sf::Vector2f position);

	void update(float deltaTime) override;

	float getKillRangeSqrd() const;
	float getEffectTimer() const;
};