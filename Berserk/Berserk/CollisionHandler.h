#pragma once

#include <iostream>
#include "Player.h"
#include "Goal.h"
#include "SMath.h"

class CollisionHandler
{
private:
	Player& player;
	Goal& goal;

	bool playerIsAtGoal;

public:
	CollisionHandler(Player& player, Goal& goal);

	void update();

	const bool playerIsCloseToGoal() const;
};