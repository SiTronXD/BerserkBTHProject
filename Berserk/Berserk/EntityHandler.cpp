#include "EntityHandler.h"

EntityHandler::EntityHandler(GameStatsHandler& gameStats)
	: player(2, 2, *this), collisionHandler(player, goal, gameStats, *this), nrOfCollectibles(0)
{

}

EntityHandler::~EntityHandler()
{
	// Collectibles
	for (int i = 0; i < nrOfCollectibles; ++i)
		delete this->collectibles[i];
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);
	this->player.update(deltaTime);

	this->collisionHandler.update();

	// Update collectibles
	for (unsigned int i = 0; i < nrOfCollectibles; ++i)
	{
		if (this->collectibles[i] != nullptr)
		{
			this->collectibles[i]->update(deltaTime);

			// Should remove?
			if (this->collectibles[i]->getShouldRemove())
				collectibleHasBeenFound(i);
		}
	}
}

void EntityHandler::placeGoal(sf::Vector2f goalPos)
{
	this->goal.setPosition(goalPos);
}

void EntityHandler::addCollectible(sf::Vector2f newCollectiblePos)
{
	if(nrOfCollectibles < MAX_NUM_COLLECTIBLES)
		this->collectibles[nrOfCollectibles++] = new Collectible(newCollectiblePos);
}

void EntityHandler::render(sf::RenderWindow& window)
{
	this->player.render(window);
}

int EntityHandler::getNumCollectibles() const
{
	return this->nrOfCollectibles;
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

void EntityHandler::fillArraysWithEntityArrays(sf::Glsl::Vec3 positionArray[], 
	sf::Glsl::Vec4 textureRectsArray[], sf::Glsl::Vec2 worldScaleArray[], int& arraySize)
{
	int currentArraySize = 0;

	// Collectibles
	for (unsigned int i = 0; i < nrOfCollectibles; ++i)
	{
		if (this->collectibles[i] != nullptr)
		{
			// Position
			positionArray[i] = this->collectibles[i]->getPositionGlsl();

			// Texture rect
			textureRectsArray[i] = this->collectibles[i]->getTextureRectGlsl();

			// Scale
			worldScaleArray[i] = this->collectibles[i]->getWorldScaleGlsl();

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
	this->collectibles[index] = collectibles[--this->nrOfCollectibles];
}
