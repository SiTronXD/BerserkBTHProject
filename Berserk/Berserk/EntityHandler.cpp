#include "EntityHandler.h"

EntityHandler::EntityHandler()
	: player(2, 2), collisionHandler(player, goal)
{
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);

	this->collisionHandler.update();
}

void EntityHandler::placeGoal(sf::Vector2f goalPos)
{
	this->goal.setPosition(goalPos);
}

const Player& EntityHandler::getPlayer()
{
	return this->player;
}

CollisionHandler& EntityHandler::getCollisionHandler()
{
	return this->collisionHandler;
}
