#include "EntityHandler.h"
#include <iostream>

void EntityHandler::removeEnemy(int index)
{
	delete this->enemies[index];
	this->enemies[index] = this->enemies[--this->nrOfEnemies];
}

void EntityHandler::enemyBleeding(sf::Vector2f enemyPos)
{
	// Stamp randomly placed particles
	float randomAngle = (rand() % 1001) * 0.001f * 2.0f * 3.1415f;
	float randomRadius = (float) (pow((rand() % 1001) * 0.001f, 3) * 16.0);

	int x = (int) (randomRadius * std::cos(randomAngle));
	int y = (int) (randomRadius * std::sin(randomAngle));

	// Stamp
	this->stampEnemyBloodIntoFloorTexture(enemyPos, sf::Vector2i(x, y));
}

EntityHandler::EntityHandler(GameStatsHandler& gameStats)
	: player(), collisionHandler(gameStats, *this),
	nrOfCollectibles(0), nrOfEnemies(0), playerIsTakingDamage(false)
{ 
	if (!this->floorEffectsTexture.create(FLOOR_EFFECTS_TEXTURE_SIZE, FLOOR_EFFECTS_TEXTURE_SIZE))
		std::cout << "Could not create floorEffectsTexture..." << std::endl;

	this->floorEffectShape.setSize(sf::Vector2f(1, 1));
	this->floorEffectShape.setFillColor(sf::Color::Red);

}

EntityHandler::~EntityHandler()
{
	// Collectibles
	for (int i = 0; i < this->nrOfCollectibles; ++i)
		delete this->collectibles[i];

	// Enemies
	for (int i = 0; i < this->nrOfEnemies; ++i)
		delete this->enemies[i];
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);
	this->player.update(deltaTime);

	// Set renderer fog color
	if (this->player.isBerserkerActive())
		this->currentFogColor = sf::Color(255, 0, 0, 255);
	else
		this->currentFogColor = sf::Color(0, 0, 0, 255);

	// Update collectibles
	for (int i = 0; i < this->nrOfCollectibles; ++i)
	{
		if (this->collectibles[i])
		{
			this->collectibles[i]->update(deltaTime);

			// Should remove?
			if (this->collectibles[i]->getShouldRemove())
				collectibleHasBeenFound(i);
		}
	}

	// Update enemies
	this->playerIsTakingDamage = false;
	for (int i = 0; i < this->nrOfEnemies; ++i)
	{
		// Update
		if (!this->enemies[i]->isDead())
			this->enemies[i]->update(deltaTime, this->player.getPosition());
		else if (this->enemies[i]->canDropBlood())
		{
			this->enemies[i]->updateBloodTimer(deltaTime);

			// Don't drop blood too fast
			if(this->enemies[i]->readyToDropBlood())
				this->enemyBleeding(this->enemies[i]->getPosition2D());
		}

		// Should remove?
		if (this->enemies[i]->getShouldRemove())
		{
			removeEnemy(i);
		}
		// Check if the enemy is doing damage
		else if(this->enemies[i]->isDoingDamage() && !this->player.isHealthDepleted() &&
			!this->player.isBerserkerActive())
		{
			this->playerIsTakingDamage = true;

			this->player.loseHealth();
		}
	}

	// Update collisions after everything has moved
	this->collisionHandler.update();
}

void EntityHandler::placeGoal(sf::Vector2f goalPos)
{
	this->goal.setPosition(goalPos);
}

void EntityHandler::placePlayer(sf::Vector2f playerPos)
{
	this->player.setPosition(playerPos);
}

void EntityHandler::placeWall(sf::Vector2i wallPos)
{
	this->collisionHandler.placeWall(wallPos);
}

void EntityHandler::addCollectible(sf::Vector2f newCollectiblePos)
{
	if(this->nrOfCollectibles < this->MAX_NUM_COLLECTIBLES)
		this->collectibles[this->nrOfCollectibles++] = new Collectible(newCollectiblePos);
}

void EntityHandler::addEnemy(sf::Vector2f newEnemyPos)
{
	if (this->nrOfEnemies < this->MAX_NUM_ENEMIES)
		this->enemies[this->nrOfEnemies++] = new Enemy(newEnemyPos);
}

void EntityHandler::render(sf::RenderWindow& window)
{
	this->player.render(window);
}

int EntityHandler::getNumCollectibles() const
{
	return this->nrOfCollectibles;
}

int EntityHandler::getNumEnemies() const
{
	return this->nrOfEnemies;
}

bool EntityHandler::isPlayerTakingDamage() const
{
	return this->playerIsTakingDamage;
}

bool EntityHandler::playerHasLost() const
{
	return this->player.isDead();
}

bool EntityHandler::playerIsCloseToGoal() const
{
	return this->collisionHandler.playerIsCloseToGoal();
}

sf::Color EntityHandler::getCurrentFogColor() const
{
	return this->currentFogColor;
}

const std::string EntityHandler::getUIMessage()
{
	return this->collisionHandler.getUIMessage();
}

Player& EntityHandler::getPlayer()
{
	return this->player;
}

Goal& EntityHandler::getGoal()
{
	return this->goal;
}

CollisionHandler& EntityHandler::getCollisionHandler()
{
	return this->collisionHandler;
}

Collectible* EntityHandler::getCollectible(int index)
{
	return this->collectibles[index];
}

Enemy* EntityHandler::getEnemy(int index)
{
	return this->enemies[index];
}

const sf::RenderTexture& EntityHandler::getFloorEffectsTexture() const
{
	return this->floorEffectsTexture;
}

void EntityHandler::fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
	sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize)
{
	int currentArraySize = 0;

	// Collectibles
	for (int i = 0; i < this->nrOfCollectibles; ++i)
	{
		if (this->collectibles[i])
		{
			// Position
			positionArray[currentArraySize] = this->collectibles[i]->getPositionGlsl();

			// Texture rect
			textureRectsArray[currentArraySize] = this->collectibles[i]->getTextureRectGlsl();

			// Scale
			worldScaleArray[currentArraySize] = this->collectibles[i]->getWorldScaleGlsl();

			// Increment size
			currentArraySize++;
		}
	}
	
	// Enemies
	for (int i = 0; i < this->nrOfEnemies; ++i)
	{
		if (this->enemies[i])
		{
			sf::Vector2f deltaPos = this->player.getPosition() - this->enemies[i]->getPosition2D();

			// Render the enemy only if it is within range
			if (SMath::dot(deltaPos, deltaPos) <= this->player.getEntityVisibleRadiusSqrd())
			{
				// Position
				positionArray[currentArraySize] = this->enemies[i]->getPositionGlsl();

				// Texture rect
				textureRectsArray[currentArraySize] = this->enemies[i]->getTextureRectGlsl();

				// Scale
				worldScaleArray[currentArraySize] = this->enemies[i]->getWorldScaleGlsl();

				// Increment size
				currentArraySize++;
			}
		}
	}

	// Grenade
	Grenade* grenade = this->player.getGrenade();
	if (grenade)
	{
		// Position
		positionArray[currentArraySize] = grenade->getPositionGlsl();

		// Texture rect
		textureRectsArray[currentArraySize] = grenade->getTextureRectGlsl();

		// Scale
		worldScaleArray[currentArraySize] = grenade->getWorldScaleGlsl();

		// Increment size
		currentArraySize++;
	}

	// Grenade explosion
	GrenadeExplosion* grenadeExplosion = this->player.getGrenadeExplosion();
	if (grenadeExplosion)
	{
		// Position
		positionArray[currentArraySize] = grenadeExplosion->getPositionGlsl();

		// Texture rect
		textureRectsArray[currentArraySize] = grenadeExplosion->getTextureRectGlsl();

		// Scale
		worldScaleArray[currentArraySize] = grenadeExplosion->getWorldScaleGlsl();

		// Increment size
		currentArraySize++;
	}

	arraySize = currentArraySize;
}

void EntityHandler::collectibleHasBeenFound(int index)
{
	delete this->collectibles[index];
	this->collectibles[index] = this->collectibles[--this->nrOfCollectibles];
}

void EntityHandler::stampEnemyBloodIntoFloorTexture(sf::Vector2f pos, sf::Vector2i pixelOffset)
{
	this->floorEffectShape.setPosition(
		(int) (pos.x * FLOOR_EFFECTS_TILE_SIZE + pixelOffset.x), 
		(int) (FLOOR_EFFECTS_TEXTURE_SIZE - (pos.y * FLOOR_EFFECTS_TILE_SIZE + pixelOffset.y))
	);

	this->floorEffectsTexture.draw(floorEffectShape);
	this->floorEffectsTexture.display();
}