#include "EntityHandler.h"
#include "Renderer.h"

void EntityHandler::removeEnemy(int index)
{
	delete this->enemies[index];
	this->enemies[index] = this->enemies[--this->nrOfEnemies];
}

EntityHandler::EntityHandler(GameStatsHandler& gameStats)
	: player(4, 4, *this), collisionHandler(player, goal, gameStats, *this),
	renderer(nullptr), nrOfCollectibles(0), nrOfEnemies(0), playerIsTakingDamage(false)
{ }

EntityHandler::~EntityHandler()
{
	// Collectibles
	for (int i = 0; i < this->nrOfCollectibles; ++i)
		delete this->collectibles[i];

	// Enemies
	for (int i = 0; i < this->nrOfEnemies; ++i)
		delete this->enemies[i];
}

void EntityHandler::setRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);
	this->player.update(deltaTime);

	// Set renderer fog color
	if (this->player.isBerserkerActive())
		this->renderer->setFogColor(sf::Color(255, 0, 0, 255));
	else
		this->renderer->setFogColor(sf::Color(0, 0, 0, 255));

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
		this->enemies[i]->update(deltaTime, this->player.getPosition());

		// Should remove?
		if (this->enemies[i]->getShouldRemove())
		{
			removeEnemy(i);
		}
		// Check if the enemy is doing damage
		else if(this->enemies[i]->isDoingDamage() && !this->player.isHealthDepleted())
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

void EntityHandler::addEnemySpawnPoint(sf::Vector2f spawnPointPos)
{
	this->spawner.addSpawnPoint(spawnPointPos);
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

Player& EntityHandler::getPlayer()
{
	return this->player;
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
