#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "ResTranslator.h"
#include "SettingsHandler.h"
#include "SMath.h"
#include "Animation.h"
#include "Grenade.h"
#include "GrenadeExplosion.h"

class EntityHandler;

class Player
{
private:
	const int BERSERK_SPRITE_WIDTH = 114;
	const int BERSERK_SPRITE_HEIGHT = 64;
	const int FIRST_PERSON_SPRITE_WIDTH = 192;
	const int FIRST_PERSON_SPRITE_HEIGHT = 64;

	const float MOVEMENT_SPEED = 6.0f;
	const float BERSERKER_MOVEMENT_SPEED = 10.0f;
	const float ATTACK_CONE_ANGLE = 3.1415f * 0.3f;
	const float MAX_ATTACK_DIST = 2.0f;

	const float SWORD_SPRITE_SCALE = 10.0f;
	const float MAX_ATTACKING_TIME = 1.0f;
	const float ABILITY_GRENADE_MAX_COOLDOWN_TIME = 1.0f;
	const float ABILITY_BERSERKER_MAX_COOLDOWN_TIME = 1.0f;
	const float MAX_BERSERKER_TIME = 2.0f;
	const float BERSERKER_ALPHA_ANIMATION_TIME_SCALE = 3.0f;

	sf::Texture swordTextureSheet;
	sf::Texture grenadeThrowTextureSheet;
	sf::Texture startBerserkTextureSheet;
	sf::Texture endBerserkTextureSheet;
	sf::Sprite berserkSprite;
	sf::Sprite handsSprite;

	sf::Vector2i monitorMiddle;
	sf::Vector2f swordPosition;
	sf::Vector2f walkStep;

	Animation* currentFpsAnimation;
	Animation* currentBerserkAnimation;
	Animation swordIdleAnimation;
	Animation swordAttackAnimation;
	Animation grenadeThrowAnimation;
	Animation startBerserkAnimation;
	Animation endBerserkAnimation;

	EntityHandler& entityHandler;

	Grenade* grenade;
	GrenadeExplosion* grenadeExplosion;

	float x;
	float y;
	float lastFrameX;
	float lastFrameY;
	float direction; // Direction is in radians
	float walkTimer;
	float isAttackingTimer;
	float berserkerActiveTimer;
	float grenadeCooldownTimer;
	float berserkerCooldownTimer;
	float berserkerAnimationAlpha;

	bool tryToExit;
	bool hasStartedAttackAnimation;
	bool startThrowAnimation;
	bool startStartBerserkAnimation;
	bool startEndBerserkAnimation;
	bool hasSpawnedGrenade;
	bool berserkerIsActive;

	void loadAnimations();
	void spawnGrenade();
	void updateAnimationLogic(float deltaTime);
	void updateFpsSpritePosition();

public:
	Player(int x, int y, EntityHandler& entityHandler);
	~Player();

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	void setPosition(sf::Vector2f newPos);

	const bool playerTriesToExit() const;
	const sf::Vector2f getPosition() const;
	const sf::Vector2f getWalkStep() const;
	const sf::Vector2f getLastFramePosition() const;
	const sf::Vector2f getLookDirectionVec() const;
	const sf::Glsl::Vec3 getOrientation() const;

	bool isAttacking() const;
	bool isBerserkerActive() const;

	float getGrenadeCooldownPercent() const;
	float getBerserkerCooldownPercent() const;
	float getPlayerCollisionBoxSize() const;
	float getAttackConeAngle() const;
	float getMaxAttackDistSqrd() const;

	Grenade* getGrenade() const;
	GrenadeExplosion* getGrenadeExplosion() const;
};