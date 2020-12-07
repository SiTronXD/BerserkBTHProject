#pragma once

#include <iostream>
#include "Player.h"
#include "Collectible.h"
#include "Goal.h"
#include "GameStatsHandler.h"
#include "SMath.h"
#include "Enemy.h"

class EntityHandler;

class CollisionHandler
{
private:
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

public:
	CollisionHandler(GameStatsHandler& gameStats, 
		EntityHandler& entityHandler);

	void update();
	void placeWall(sf::Vector2i pos);

	const std::string getUIMessage();
	bool playerIsCloseToGoal() const;
};