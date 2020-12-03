#pragma once

#include "Player.h"
#include "Goal.h"
#include "Collectible.h"
#include "Grenade.h"
#include "CollisionHandler.h"
#include "GameStatsHandler.h"

class EntityHandler
{
private:
	static const int MAX_NUM_COLLECTIBLES = 5;
	int nrOfCollectibles;

	Player player;
	Goal goal;
	CollisionHandler collisionHandler;
	Collectible* collectibles[MAX_NUM_COLLECTIBLES] { };

public:
	EntityHandler(GameStatsHandler& gameStats);
	~EntityHandler();

	void placeGoal(sf::Vector2f goalPos);
	void addCollectible(sf::Vector2f newCollectiblePos);
	void fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
		sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize);
	void collectibleHasBeenFound(int index);

	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	int getNumCollectibles() const;

	Player& getPlayer();
	CollisionHandler& getCollisionHandler();
	Collectible* getCollectible(int index);
};