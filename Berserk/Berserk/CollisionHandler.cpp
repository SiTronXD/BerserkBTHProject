#include "CollisionHandler.h"

CollisionHandler::CollisionHandler(Player& player, Goal& goal)
	: player(player), goal(goal), playerIsAtGoal(false)
{ }

void CollisionHandler::update()
{
	// Check if the player is close enough to the goal
	sf::Vector2f playerToGoal = goal.getPosition() - player.getPlayerPosition();

	this->playerIsAtGoal = SMath::dot(playerToGoal, playerToGoal) < goal.getRadiusSqrd();
}

const bool CollisionHandler::playerIsCloseToGoal() const
{
	return this->playerIsAtGoal;
}
