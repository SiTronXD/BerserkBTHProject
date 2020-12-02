#pragma once

#include <iostream>
#include "Player.h"
#include "Goal.h"
#include "SMath.h"

class CollisionHandler
{
private:
	static const int MAX_MAP_SIZE = 128;

	Player& player;
	Goal& goal;

	bool playerIsAtGoal;
	bool mapWalls[MAX_MAP_SIZE][MAX_MAP_SIZE] {};

	bool isPlayerCollidingWall(float playerX, float playerY, int wallX, int wallY);

public:
	CollisionHandler(Player& player, Goal& goal);

	void update();
	void setWallAt(sf::Vector2i pos);

	const bool playerIsCloseToGoal() const;
};