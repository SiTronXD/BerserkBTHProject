#pragma once

#include "Player.h"
#include "Goal.h"

class EntityHandler
{
private:
	Player player;
	Goal goal;

public:
	EntityHandler();

	void update(float deltaTime);

	void placeGoal(sf::Vector2f goalPos);

	const Player& getPlayer();
};