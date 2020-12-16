#pragma once

#include "Player.h"
#include "Goal.h"
#include "Collectible.h"
#include "Grenade.h"
#include "Enemy.h"
#include "CollisionHandler.h"
#include "GameStatsHandler.h"

class EntityHandler
{
private:
	static const int MAX_NUM_COLLECTIBLES = 5;
	static const int MAX_NUM_ENEMIES = 128;

	sf::Color currentFogColor;

	Player player;
	Goal goal;
	CollisionHandler collisionHandler;
	Collectible* collectibles[MAX_NUM_COLLECTIBLES] { };
	Enemy* enemies[MAX_NUM_ENEMIES] { };

	int nrOfCollectibles;
	int nrOfEnemies;

	bool playerIsTakingDamage;

	void removeEnemy(int index);

	EntityHandler(const EntityHandler& other) = delete;
	EntityHandler operator=(const EntityHandler& other) = delete;

public:
	EntityHandler(GameStatsHandler& gameStats);
	~EntityHandler();

	void placeGoal(sf::Vector2f goalPos);
	void placePlayer(sf::Vector2f playerPos);
	void placeWall(sf::Vector2i wallPos);
	void addCollectible(sf::Vector2f newCollectiblePos);
	void addEnemy(sf::Vector2f newEnemyPos);
	void fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
		sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize);
	void collectibleHasBeenFound(int index);

	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	int getNumCollectibles() const;
	int getNumEnemies() const;

	bool isPlayerTakingDamage() const;
	bool playerHasLost() const;
	bool playerIsCloseToGoal() const;

	sf::Color getCurrentFogColor() const;

	const std::string getUIMessage();

	Player& getPlayer();
	Goal& getGoal();
	CollisionHandler& getCollisionHandler();
	Collectible* getCollectible(int index);
	Enemy* getEnemy(int index);
};