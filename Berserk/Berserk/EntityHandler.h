#pragma once

#include "Player.h"
#include "Goal.h"
#include "Collectible.h"
#include "Spawner.h"
#include "Grenade.h"
#include "Enemy.h"
#include "CollisionHandler.h"
#include "GameStatsHandler.h"

class Renderer;

class EntityHandler
{
private:
	static const int MAX_NUM_COLLECTIBLES = 5;
	static const int MAX_NUM_ENEMIES = 32;

	Player player;
	Goal goal;
	CollisionHandler collisionHandler;
	Spawner spawner;
	Collectible* collectibles[MAX_NUM_COLLECTIBLES] { };
	Enemy* enemies[MAX_NUM_ENEMIES] { };
	Renderer* renderer;

	int nrOfCollectibles;
	int nrOfEnemies;

	bool playerIsTakingDamage;

	void removeEnemy(int index);

public:
	EntityHandler(GameStatsHandler& gameStats);
	~EntityHandler();

	void setRenderer(Renderer* renderer);

	void placeGoal(sf::Vector2f goalPos);
	void placePlayer(sf::Vector2f playerPos);
	void addCollectible(sf::Vector2f newCollectiblePos);
	void addEnemy(sf::Vector2f newEnemyPos);
	void addEnemySpawnPoint(sf::Vector2f spawnPointPos);
	void fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
		sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize);
	void collectibleHasBeenFound(int index);

	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	int getNumCollectibles() const;
	int getNumEnemies() const;

	bool isPlayerTakingDamage() const;
	bool playerHasLost() const;

	Player& getPlayer();
	CollisionHandler& getCollisionHandler();
	Collectible* getCollectible(int index);
	Enemy* getEnemy(int index);
};