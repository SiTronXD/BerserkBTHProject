#include "CollisionHandler.h"
#include "EntityHandler.h"

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
	: player(player), goal(goal), gameStats(gameStats), entityHandler(entityHandler), playerIsAtGoal(false)
{ }

void CollisionHandler::update()
{
	// Check if the player is close enough to the goal
	sf::Vector2f playerToGoal = goal.getPosition() - player.getPlayerPosition();

	this->playerIsAtGoal = SMath::dot(playerToGoal, playerToGoal) < goal.getRadiusSqrd();


	// Player collision with walls
	sf::Vector2f lastPlayerPos = this->player.getPlayerLastFramePosition();
	sf::Vector2f playerWalkStep = this->player.getPlayerWalkStep();
	int playerTilePosX = (int)lastPlayerPos.x;
	int playerTilePosY = (int)lastPlayerPos.y;

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			sf::Vector2f currentPlayerPos = this->player.getPlayerPosition();
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
				this->player.setPlayerPosition(newPlayerPos);
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
				player.getPlayerPosition();

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
