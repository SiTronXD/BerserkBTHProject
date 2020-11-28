#include "EntityHandler.h"

EntityHandler::EntityHandler()
	: player(2, 2), collisionHandler(player, goal), nrOfCollectibles(0)
{

}

EntityHandler::~EntityHandler()
{
	for (unsigned int i = 0; i < nrOfCollectibles; ++i)
		delete this->collectibles[i];
}

void EntityHandler::update(float deltaTime)
{
	this->player.handleInput(deltaTime);
	this->player.update(deltaTime);

	this->collisionHandler.update();
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

Player& EntityHandler::getPlayer()
{
	return this->player;
}

CollisionHandler& EntityHandler::getCollisionHandler()
{
	return this->collisionHandler;
}

const sf::Texture& EntityHandler::getRenderEntityTexture() const
{
	return this->collectibles[0]->getTexture();
}

void EntityHandler::fillArrayWithEntityPositions(sf::Glsl::Vec3 positionArray[], int& arraySize)
{
	for(unsigned int i = 0; i < nrOfCollectibles; ++i)
		positionArray[i] = this->collectibles[i]->getPosition();

	arraySize = nrOfCollectibles;
}
