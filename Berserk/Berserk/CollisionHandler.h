#pragma once

#include <iostream>
#include "Player.h"
#include "Collectible.h"
#include "Goal.h"
#include "GameStatsHandler.h"
#include "SMath.h"

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

	bool isPlayerCollidingWall(float playerX, float playerY, int wallX, int wallY);

public:
	CollisionHandler(Player& player, Goal& goal, GameStatsHandler& gameStats, 
		EntityHandler& entityHandler);

	void update();
	void setWallAt(sf::Vector2i pos);

	const std::string getUIMessage();
	const bool playerIsCloseToGoal() const;
};