#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Engine/ResTranslator.h"
#include "../Engine/SettingsHandler.h"
#include "../Engine/SMath.h"
#include "../Engine/Animation.h"
#include "../Entities/Grenade.h"
#include "../Entities/GrenadeExplosion.h"

class Player
{
private:
	const int BERSERK_SPRITE_WIDTH = 114;
	const int BERSERK_SPRITE_HEIGHT = 64;
	const int FIRST_PERSON_SPRITE_WIDTH = 192;
	const int FIRST_PERSON_SPRITE_HEIGHT = 64;

	const float HP_DECREASE_AMOUNT = 25;
	const float HP_INCREASE_AMOUNT = 1;
	const float MOVEMENT_SPEED_DEFAULT = 6.0f;
	const float MOVEMENT_SPEED_BERSERKER = 9.0f;
	const float MOVEMENT_SPEED_ATTACKING_SCALE = 1.7f;
	const float MAX_ATTACK_COOLDOWN_TIME_DEFAULT = 1.0f;
	const float MAX_ATTACK_COOLDOWN_TIME_BERSERKER = 0.0f;

	const float ATTACK_CONE_ANGLE = 3.1415f * 0.3f;
	const float MAX_ATTACK_DIST = 1.0f;
	const float MAX_ATTACK_TIME = 0.5f;
	const float MAX_DIE_ANIMATION_TIME = 0.9f;
	const float MAX_POST_DIE_ANIMATION_TIME = 1.0f;

	const float SWORD_SPRITE_SCALE = 10.0f;
	const float MAX_ATTACKING_TIME = 1.0f;
	const float MAX_BERSERKER_TIME = 8.0f;
	const float BERSERKER_ALPHA_ANIMATION_TIME_SCALE = 3.0f;
	const float ENTITY_VISIBLE_RADIUS = 16.0f;

	const int ABILITY_GRENADE_REQUIRED_NUM_KILLS = 10;
	const int ABILITY_BERSERKER_REQUIRED_NUM_KILLS = 20;

	sf::Texture swordTextureSheet;
	sf::Texture grenadeThrowTextureSheet;
	sf::Texture startBerserkTextureSheet;
	sf::Texture endBerserkTextureSheet;
	sf::Sprite berserkSprite;
	sf::Sprite handsSprite;

	sf::Sound soundPlayer;
	sf::SoundBuffer swingSwordSound;
	sf::SoundBuffer startSwingingSwordSound;
	sf::SoundBuffer throwGrenadeSound;
	sf::SoundBuffer berserkStartSound;
	sf::SoundBuffer berserkEndSound;
	sf::SoundBuffer damageTakenSound;

	sf::Vector2i monitorMiddle;
	sf::Vector2f swordPosition;
	sf::Vector2f walkStep;

	Animation* currentFpsAnimation;
	Animation* currentBerserkAnimation;
	Animation anims[5];

	Grenade* grenade;
	GrenadeExplosion* grenadeExplosion;

	float x;
	float y;
	float z;
	float lastFrameX;
	float lastFrameY;
	float direction;	// Direction is in radians
	float roll;			// Also in radians
	float walkTimer;
	float isAttackingTimer;
	float attackCooldownTimer;
	float berserkerActiveTimer;
	float berserkerAnimationAlpha;
	float health;
	float dieTimer;

	int grenadeCooldownCounter;
	int berserkerCooldownCounter;

	bool tryToExit;
	bool hasStartedAttackAnimation;
	bool startThrowAnimation;
	bool startStartBerserkAnimation;
	bool startEndBerserkAnimation;
	bool hasSpawnedGrenade;
	bool berserkerIsActive;
	bool dead;

	void loadAnimations();
	void loadSounds();
	void spawnGrenade();
	void updateAnimationLogic(float deltaTime);
	void updateFpsSpritePosition();
	void playSound(sf::SoundBuffer& sfx);

	Player(const Player& other) = delete;
	Player operator=(const Player& other) = delete;

public:
	Player();
	~Player();

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	void setPosition(sf::Vector2f newPos);
	void gainHealth();
	void loseHealth();
	void killedEnemy();

	const bool playerTriesToExit() const;
	const sf::Vector2f getPosition() const;
	const sf::Vector2f getWalkStep() const;
	const sf::Vector2f getLastFramePosition() const;
	const sf::Vector2f getLookDirectionVec() const;
	const sf::Glsl::Vec3 getPositionForRenderer() const;
	const sf::Glsl::Vec2 getRotationForRenderer() const;

	bool isAttacking() const;
	bool isBerserkerActive() const;
	bool isHealthDepleted() const;
	bool isDead() const;

	int getCurrentHealth() const;

	float getGrenadeCooldownPercent() const;
	float getBerserkerCooldownPercent() const;
	float getPlayerCollisionBoxSize() const;
	float getAttackConeAngle() const;
	float getMaxAttackDistSqrd() const;
	float getBerserkerBlackBarAlpha() const;
	float getEntityVisibleRadiusSqrd() const;

	Grenade* getGrenade() const;
	GrenadeExplosion* getGrenadeExplosion() const;
};