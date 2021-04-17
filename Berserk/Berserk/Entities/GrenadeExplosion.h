#pragma once

#include "../Engine/RenderEntity.h"

class GrenadeExplosion : public RenderEntity
{
private:
	const float MAX_GROW_TIME = 0.5f;
	const float MAX_SHRINK_TIME = 0.5f;
	const float MAX_ALIVE_TIME = 3.0f;
	const float KILL_RANGE = 8.0f;

	sf::Sound enemyDiesSoundPlayer;
	sf::SoundBuffer explosionSound;
	sf::SoundBuffer enemyDiedSound;

	float aliveTimer;
	float enemyDiedSoundTimer;
	float nextEnemyDiedSoundMaxTime;

	void setRandomEnemyDiedSoundTime();
	void setSize(float percent);

public:
	GrenadeExplosion(sf::Vector2f position);

	void update(float deltaTime) override;

	float getKillRangeSqrd() const;
	float getEffectTimer() const;
};