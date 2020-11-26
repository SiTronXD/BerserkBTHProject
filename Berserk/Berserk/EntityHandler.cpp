#include "EntityHandler.h"

EntityHandler::EntityHandler()
	: player(2, 2)
{
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);
}

void EntityHandler::placeGoal(sf::Vector2f goalPos)
{
	this->goal.setPosition(goalPos);
}

const Player& EntityHandler::getPlayer()
{
	return this->player;
}