#include "CollisionHandler.h"
#include "EntityHandler.h"

/*
bool CollisionHandler::canEnemySeeTargetPlotLineLow(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;

	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	int D = (2 * dy) - dx;
	int y = y0;

	for (int x = x0; x <= x1; ++x)
	{
		if (this->mapWalls[x][y])
		{
			std::cout << this->mapWalls[x][y] << std::endl;

			return false;
		}

		if (D > 0)
		{
			y += yi;
			D += (2 * (dy - dx));
		}
		else
			D += 2 * dy;
	}

	return true;
}

bool CollisionHandler::canEnemySeeTargetPlotLineHigh(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;

	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = x0;

	for (int y = y0; y <= y0; ++y)
	{
		if (this->mapWalls[x][y])
		{
			return false;
		}

		if (D > 0)
		{
			x += xi;
			D += 2 * (dx - dy);
		}
		else
			D += 2 * dx;
	}

	return true;
}

bool CollisionHandler::canEnemySeeTarget(sf::Vector2f enemyPosition, sf::Vector2f targetPosition)
{
	int x0 = (int) enemyPosition.x;
	int y0 = (int) enemyPosition.y;
	int x1 = (int) targetPosition.x;
	int y1 = (int) targetPosition.y;

	bool result = true;

	if (abs(y1 - y0) < abs(x1 - x0))
	{
		if (x0 > x1)
			result = this->canEnemySeeTargetPlotLineLow(x1, y1, x0, y0);
		else
			result = this->canEnemySeeTargetPlotLineLow(x0, y0, x1, y1);
	}
	else
	{
		if (y0 > y1)
			result = this->canEnemySeeTargetPlotLineHigh(x1, y1, x0, y0);
		else
			result = this->canEnemySeeTargetPlotLineHigh(x0, y0, x1, y1);
	}

	return result;
}
*/


bool CollisionHandler::isPlayerCollidingWall(float playerX, float playerY, int wallX, int wallY)
{
	bool colliding = false;
	float playerSize = this->player.getPlayerCollisionBoxSize();

	// Check only if this tile is a wall
	if(this->mapWalls[wallX][wallY])
		colliding = (playerX + playerSize > wallX && playerX - playerSize < wallX + 1 &&
			playerY + playerSize > wallY && playerY - playerSize < wallY + 1);

	return colliding;
}

CollisionHandler::CollisionHandler(Player& player, Goal& goal, GameStatsHandler& gameStats, 
	EntityHandler& entityHandler)
	: player(player), goal(goal), gameStats(gameStats), entityHandler(entityHandler), 
	playerIsAtGoal(false)
{ }

void CollisionHandler::update()
{
	// Check if the player is close enough to the goal
	sf::Vector2f playerToGoal = goal.getPosition() - player.getPosition();

	this->playerIsAtGoal = SMath::dot(playerToGoal, playerToGoal) < goal.getRadiusSqrd();


	// Player collision with walls
	sf::Vector2f lastPlayerPos = this->player.getLastFramePosition();
	sf::Vector2f playerWalkStep = this->player.getWalkStep();
	int playerTilePosX = (int)lastPlayerPos.x;
	int playerTilePosY = (int)lastPlayerPos.y;

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			sf::Vector2f currentPlayerPos = this->player.getPosition();
			sf::Vector2f newPlayerPos = currentPlayerPos;

			int tileX = SMath::clamp((int)(playerTilePosX + x), 0, MAX_MAP_SIZE);
			int tileY = SMath::clamp((int)(playerTilePosY + y), 0, MAX_MAP_SIZE);


			// Check for collision on the X-axis
			if (isPlayerCollidingWall(lastPlayerPos.x + playerWalkStep.x, lastPlayerPos.y, tileX, tileY))
			{
				float playerBoxSize = this->player.getPlayerCollisionBoxSize();

				// Move in X
				if (lastPlayerPos.x - (tileX + 0.5f) > 0.0f)
					newPlayerPos.x = tileX + 1 + playerBoxSize;
				else
					newPlayerPos.x = tileX - playerBoxSize;

				// Update before checking on the Y-axis
				lastPlayerPos.x = newPlayerPos.x;
			}

			// Check for collision on the Y-axis
			if (isPlayerCollidingWall(lastPlayerPos.x, lastPlayerPos.y + playerWalkStep.y, tileX, tileY))
			{
				float playerBoxSize = this->player.getPlayerCollisionBoxSize();

				// Move in X
				if (lastPlayerPos.y - (tileY + 0.5f) > 0.0f)
					newPlayerPos.y = tileY + 1 + playerBoxSize;
				else
					newPlayerPos.y = tileY - playerBoxSize;
			}

			// Apply new position
			if(newPlayerPos != currentPlayerPos)
				this->player.setPosition(newPlayerPos);
		}
	}


	// Player collision with collectible
	for (int i = 0; i < this->entityHandler.getNumCollectibles(); ++i)
	{
		Collectible* currentCollectible = this->entityHandler.getCollectible(i);

		if (currentCollectible != nullptr)
		{
			// Collect info
			sf::Vector2f collectibleVec = sf::Vector2f(currentCollectible->getPosition2D().x, currentCollectible->getPosition2D().y);
			sf::Vector2f playerToCollectible = collectibleVec -
				player.getPosition();

			// Check if the is player close enough
			if (SMath::dot(playerToCollectible, playerToCollectible) < currentCollectible->getRadiusSqrd())
			{
				// Remove collectible
				currentCollectible->flagShouldRemove();

				// Flag Game Stats
				this->gameStats.foundCollectible();

				// UI
				this->currentUIMessage = "YOU HAVE FOUND A SECRET SKULL KNIGHT \n                             (" + 
					std::to_string(this->gameStats.getNumCollected()) + "/" + std::to_string(this->gameStats.getMaxNumCollectibles()) + ")";
			}
		}
	}


	// Player is attacking
	if (this->player.isAttacking())
	{
		for (int i = 0; i < this->entityHandler.getNumEnemies(); ++i)
		{
			Enemy* currentEnemy = this->entityHandler.getEnemy(i);

			sf::Vector2f playerToEnemy = currentEnemy->getPosition2D() - this->player.getPosition();

			// If the enemy is close enough
			if (SMath::dot(playerToEnemy, playerToEnemy) <= this->player.getMaxAttackDistSqrd())
			{
				SMath::vectorNormalize(playerToEnemy);

				// Calculate angle between playerToEnemy and player look direction
				float deltaAngle = std::acos(
					SMath::dot(playerToEnemy, this->player.getLookDirectionVec())
				);

				// If player is looking towards enemy
				if (deltaAngle <= this->player.getAttackConeAngle() * 0.5f)
					currentEnemy->kill();
			}
		}
	}
}

void CollisionHandler::setWallAt(sf::Vector2i pos)
{
	if (pos.x >= 0 && pos.x < MAX_MAP_SIZE && pos.y >= 0 && pos.y < MAX_MAP_SIZE)
		this->mapWalls[pos.x][pos.y] = true;
}

const std::string CollisionHandler::getUIMessage()
{
	std::string lastFrameMessage = this->currentUIMessage;

	this->currentUIMessage = "";

	return lastFrameMessage;
}

const bool CollisionHandler::playerIsCloseToGoal() const
{
	return this->playerIsAtGoal;
}
