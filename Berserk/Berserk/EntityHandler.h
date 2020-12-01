#pragma once

#include "Player.h"
#include "Goal.h"
#include "Collectible.h"
#include "CollisionHandler.h"

class EntityHandler
{
private:
	static const int MAX_NUM_COLLECTIBLES = 5;
	int nrOfCollectibles;

	Player player;
	Goal goal;
	CollisionHandler collisionHandler;
	Collectible* collectibles[MAX_NUM_COLLECTIBLES] { nullptr };

public:
	EntityHandler();
	~EntityHandler();

	void placeGoal(sf::Vector2f goalPos);
	void addCollectible(sf::Vector2f newCollectiblePos);
	void fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
		sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize);

	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	Player& getPlayer();
	CollisionHandler& getCollisionHandler();
};