#pragma once

#include "Player.h"

class EntityHandler
{
private:
	Player player;

public:
	EntityHandler();

	void update(float deltaTime);

	const Player& getPlayer();
};