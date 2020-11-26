#include "EntityHandler.h"

EntityHandler::EntityHandler()
	: player(2, 2)
{
}

void EntityHandler::update(float deltaTime)
{
	player.handleInput(deltaTime);
}

const Player& EntityHandler::getPlayer()
{
	return this->player;
}