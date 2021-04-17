#pragma once

#include "../Engine/RenderEntity.h"

class Enemy : public RenderEntity
{
private:
	const float MAX_ATTACK_DIST = 1.3f;
	const float MAX_PLAYER_VISIBLE_DIST = 10.0f;
	const float MAX_CAN_DROP_BLOOD_TIME = 5.0f;
	const float MAX_READY_TO_DROP_BLOOD_TIME = 0.005f;
	const float MOVEMENT_SPEED = 3.0f;

	sf::Vector2f lastPos;
	sf::Vector2f lastFramePos;
	sf::Vector2f walkStep;

	sf::SoundBuffer deadSound;
	sf::SoundBuffer noiseSound;

	float noiseSoundTimer;
	float nextNoiseSoundMaxTime;
	float caughtTime;
	float dropBloodTimer;
	float readyToDropBloodTimer;

	int enemyType;
	int lastAttackFrameIndex;

	bool dead;
	bool doDamage;
	bool canMove;
	bool justDied;

	void setNextSoundMaxTime();

public:
	Enemy(sf::Vector2f startPosition);

	void update(float deltaTime, sf::Vector2f targetPosition);
	void updateBloodTimer(float deltaTime);

	void kill(bool playDeadSound = true);
	void caughtInExplosion(float effectTimer, sf::Vector2f explosionPos);
	void resetCaughtTime();
	void resetJustDied();

	sf::Vector2f getLastFramePosition() const;
	sf::Vector2f getWalkStep() const;

	float getCollisionBoxSize() const;

	bool isDoingDamage() const;
	bool isDead() const;
	bool hasJustDied() const;
	bool canDropBlood() const;
	bool readyToDropBlood();
};