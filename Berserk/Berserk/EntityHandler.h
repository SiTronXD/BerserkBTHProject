#pragma once

#include "Player.h"
#include "Goal.h"
#include "CollisionHandler.h"

class EntityHandler
{
private:
	Player player;
	Goal goal;
	CollisionHandler collisionHandler;

public:
	EntityHandler();

	void update(float deltaTime);
	void placeGoal(sf::Vector2f goalPos);

	void render(sf::RenderWindow& window);

	const Player& getPlayer();
	CollisionHandler& getCollisionHandler();
};