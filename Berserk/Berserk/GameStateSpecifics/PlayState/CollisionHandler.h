#pragma once

#include "../../Entities/Player.h"
#include "../../Entities/Collectible.h"
#include "../../Entities/Goal.h"
#include "../../Entities/Enemy.h"
#include "../../GameStateSpecifics/GameStatsHandler.h"
#include "../../Engine/SMath.h"

class EntityHandler;

class CollisionHandler
{
private:
	static const float EPSILON;

	static const int MAX_MAP_SIZE = 128;

	Player& player;
	Goal& goal;
	GameStatsHandler& gameStats;
	EntityHandler& entityHandler;

	std::string currentUIMessage;

	bool playerIsAtGoal;
	bool mapWalls[MAX_MAP_SIZE][MAX_MAP_SIZE] {};

	bool isObjectCollidingWall(float collisionBoxSize, float objectX, float objectY, 
		int wallX, int wallY);

	sf::Vector2f getNonCollidingPosition(sf::Vector2f currentPos,
		sf::Vector2f lastPos, sf::Vector2f walkStep, float halfCollisionBoxSize);

	CollisionHandler(const CollisionHandler& other) = delete;
	CollisionHandler operator=(const CollisionHandler& other) = delete;

public:
	CollisionHandler(GameStatsHandler& gameStats, 
		EntityHandler& entityHandler);

	void update();
	void placeWall(sf::Vector2i pos);

	const std::string getUIMessage();
	bool playerIsCloseToGoal() const;
};