#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "ResTranslator.h"
#include "SettingsHandler.h"
#include "SMath.h"
#include "Animation.h"

class Player
{
private:
	const int SWORD_SPRITE_WIDTH = 192;
	const int SWORD_SPRITE_HEIGHT = 64;

	const float MOVEMENT_SPEED = 6.0f;
	const float SWORD_SPRITE_SCALE = 10.0f;
	const float MAX_ATTACKING_TIME = 1.0f;
	const float ABILITY_GRENADE_MAX_COOLDOWN_TIME = 4.0f;
	const float ABILITY_BERSERKER_MAX_COOLDOWN_TIME = 8.0f;

	sf::Texture swordTextureSheet;
	sf::Sprite swordSprite;

	sf::Vector2i monitorMiddle;
	sf::Vector2f swordPosition;

	Animation* currentSwordAnimation;
	Animation swordIdleAnimation;
	Animation swordAttackAnimation;

	float x;
	float y;
	float direction; // Direction is in radians
	float walkTimer;
	float isAttackingTimer;
	float grenadeCooldownTimer;
	float berserkerCooldownTimer;

	bool tryToExit;
	bool hasStartedAttackAnimation;

	void loadAnimations();
	void updateSwordAnimationLogic(float deltaTime);
	void updateSwordPosition();

public:
	Player(int x, int y);

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	const bool playerTriesToExit() const;
	const sf::Vector2f getPlayerPosition() const;
	const sf::Glsl::Vec3 getPlayerCamera() const;

	float getGrenadeCooldownPercent() const;
	float getBerserkerCooldownPercent() const;
};