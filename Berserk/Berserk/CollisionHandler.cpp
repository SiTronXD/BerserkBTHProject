#include "CollisionHandler.h"
#include "EntityHandler.h"

bool CollisionHandler::isObjectCollidingWall(float halfCollisionBoxSize, float objectX, float objectY, int wallX, int wallY)
{
	bool colliding = false;

	// Check only if this tile is a wall
	if(this->mapWalls[wallX][wallY])
		colliding = (objectX + halfCollisionBoxSize > wallX && objectX - halfCollisionBoxSize < wallX + 1 &&
			objectY + halfCollisionBoxSize > wallY && objectY - halfCollisionBoxSize < wallY + 1);

	return colliding;
}

sf::Vector2f CollisionHandler::getNonCollidingPosition(sf::Vector2f currentPos,
	sf::Vector2f lastPos, sf::Vector2f walkStep, float halfCollisionBoxSize)
{
	sf::Vector2f newPos = currentPos;

	int objectTilePosX = (int)lastPos.x;
	int objectTilePosY = (int)lastPos.y;

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			int tileX = SMath::clamp((int)(objectTilePosX + x), 0, MAX_MAP_SIZE);
			int tileY = SMath::clamp((int)(objectTilePosY + y), 0, MAX_MAP_SIZE);

			// Check for collision on the X-axis
			if (isObjectCollidingWall(halfCollisionBoxSize, lastPos.x + walkStep.x,
				lastPos.y, tileX, tileY))
			{
				// Move in X
				if (lastPos.x - (tileX + 0.5f) > 0.0f)
					newPos.x = tileX + 1 + halfCollisionBoxSize;
				else
					newPos.x = tileX - halfCollisionBoxSize;
			}

			// Check for collision on the Y-axis
			if (isObjectCollidingWall(halfCollisionBoxSize, lastPos.x, lastPos.y + walkStep.y,
				tileX, tileY))
			{
				// Move in X
				if (lastPos.y - (tileY + 0.5f) > 0.0f)
					newPos.y = tileY + 1 + halfCollisionBoxSize;
				else
					newPos.y = tileY - halfCollisionBoxSize;
			}
		}
	}

	return newPos;
}

CollisionHandler::CollisionHandler(GameStatsHandler& gameStats, 
	EntityHandler& entityHandler)
	: player(entityHandler.getPlayer()), goal(entityHandler.getGoal()), 
	gameStats(gameStats), entityHandler(entityHandler), playerIsAtGoal(false)
{ }

void CollisionHandler::update()
{
	Grenade* grenade = this->player.getGrenade();
	if (grenade)
	{
		grenade->setPosition(
			this->getNonCollidingPosition(
				grenade->getPosition2D(),
				grenade->getLastFramePosition(),
				grenade->getWalkStep(),
				grenade->getCollisionBoxSize()
			)
		);
	}

	// Check if the player is close enough to the goal
	sf::Vector2f playerToGoal = goal.getPosition() - player.getPosition();

	this->playerIsAtGoal = SMath::dot(playerToGoal, playerToGoal) < goal.getRadiusSqrd();

	// Player collision with walls
	this->player.setPosition(
		this->getNonCollidingPosition(
			this->player.getPosition(),
			this->player.getLastFramePosition(),
			this->player.getWalkStep(),
			this->player.getPlayerCollisionBoxSize()
		)
	);

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
			if (SMath::dot(playerToCollectible, playerToCollectible) < currentCollectible->getRadiusSqrd() &&
				!currentCollectible->hasBeenFound())
			{
				// Flag collectible
				currentCollectible->found();

				// Flag Game Stats
				this->gameStats.foundCollectible();

				// UI
				this->currentUIMessage = "YOU HAVE FOUND A SECRET SKULL KNIGHT \n                             (" + 
					std::to_string(this->gameStats.getNumCollected()) + "/" + std::to_string(this->gameStats.getMaxNumCollectibles()) + ")";
			}
		}
	}

	bool killedEnemyThisFrame = false;
	for (int i = 0; i < this->entityHandler.getNumEnemies(); ++i)
	{
		Enemy* currentEnemy = this->entityHandler.getEnemy(i);

		// Enemy collision with walls
		if (!currentEnemy->isDead())
		{
			currentEnemy->setPosition(
				this->getNonCollidingPosition(
					currentEnemy->getPosition2D(),
					currentEnemy->getLastFramePosition(),
					currentEnemy->getWalkStep(),
					currentEnemy->getCollisionBoxSize()
				)
			);
		}

		// Check if the enemy is caught within the explosion
		GrenadeExplosion* grenadeExplosion = this->player.getGrenadeExplosion();
		if (grenadeExplosion)
		{
			// Check if the enemy is within range
			sf::Vector2f grenadeToEnemy = currentEnemy->getPosition2D() -
				grenadeExplosion->getPosition2D();
			if (SMath::dot(grenadeToEnemy, grenadeToEnemy) <= grenadeExplosion->getKillRangeSqrd())
			{
				currentEnemy->caughtInExplosion(
					grenadeExplosion->getEffectTimer(),
					grenadeExplosion->getPosition2D() + player.getLookDirectionVec() * 1.0f // Move target behind explosion
				);
			}
		}
		else
			currentEnemy->resetCaughtTime();

		// Player is attacking
		if (this->player.isAttacking())
		{
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
				if (deltaAngle <= this->player.getAttackConeAngle() * 0.5f &&
					!currentEnemy->isDead())
				{
					// Kill enemy and play sound for 1 single enemy this frame
					currentEnemy->kill(!killedEnemyThisFrame);

					// Player gains health
					this->player.gainHealth();

					killedEnemyThisFrame = true;
				}
			}
		}
	}
}

void CollisionHandler::placeWall(sf::Vector2i pos)
{
	if (pos.x >= 0 && pos.x < this->MAX_MAP_SIZE && pos.y >= 0 && pos.y < this->MAX_MAP_SIZE)
		this->mapWalls[pos.x][pos.y] = true;
}

const std::string CollisionHandler::getUIMessage()
{
	std::string lastFrameMessage = this->currentUIMessage;

	this->currentUIMessage = "";

	return lastFrameMessage;
}

bool CollisionHandler::playerIsCloseToGoal() const
{
	return this->playerIsAtGoal;
}
