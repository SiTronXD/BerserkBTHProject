#pragma once

#include "RenderEntity.h"

class Enemy : public RenderEntity
{
private:
	const float MAX_ATTACK_DIST = 1.3f;
	const float MAX_PLAYER_VISIBLE_DIST = 10.0f;
	const float MOVEMENT_SPEED = 3.0f;

	sf::Vector2f lastPos;

	int enemyType;
	int lastAttackFrameIndex;

	bool dead;
	bool doDamage;
	bool canMove;

public:
	Enemy(sf::Vector2f startPosition);

	void update(float deltaTime, sf::Vector2f targetPosition);

	void kill();
	void caughtInExplosion(float effectTimer, sf::Vector2f explosionPos);

	bool isDoingDamage() const;
	bool isDead() const;
};