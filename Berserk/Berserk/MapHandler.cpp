#include "MapHandler.h"

void MapHandler::loadEntitiesFromMap(EntityHandler& entityHandler)
{
	const sf::Color WALL_COLOR = sf::Color(255, 255, 255, 255);
	const sf::Color GOAL_COLOR = sf::Color(0, 255, 0, 255);
	const sf::Color COLLECTIBLE_COLOR = sf::Color(255, 255, 0, 255);

	CollisionHandler& collisionHandler = entityHandler.getCollisionHandler();

	// Load image and size
	sf::Image image(this->map.copyToImage());
	unsigned int mapWidth = image.getSize().x;
	unsigned int mapHeight = image.getSize().y;

	// Go through each pixel in the map
	for (unsigned int y = 0; y < mapHeight; ++y)
	{
		for (unsigned int x = 0; x < mapWidth; ++x)
		{
			sf::Color pixelColor = image.getPixel(x, y);
			
			// Wall
			if(this->evaluatePixel(pixelColor, WALL_COLOR))
				collisionHandler.setWallAt(sf::Vector2i(x, y));

			// Goal
			if (this->evaluatePixel(pixelColor, GOAL_COLOR))
			{
				entityHandler.placeGoal(sf::Vector2f(x + 0.5f, y + 0.5f));

				// Set wall
				collisionHandler.setWallAt(sf::Vector2i(x, y));
			}

			// Collectible
			if (this->evaluatePixel(pixelColor, COLLECTIBLE_COLOR))
				entityHandler.addCollectible(sf::Vector2f(x + 0.5f, y + 0.5f));
		}
	}
}

bool MapHandler::evaluatePixel(sf::Color currentPixel, sf::Color targetPixel)
{
	sf::Vector3f currentPixelCol(currentPixel.r / 255.0f, currentPixel.g / 255.0f, currentPixel.b / 255.0f);
	sf::Vector3f targetPixelCol(targetPixel.r / 255.0f, targetPixel.g / 255.0f, targetPixel.b / 255.0f);

	sf::Vector3f targetCol = currentPixelCol - targetPixelCol;

	return SMath::dot(targetCol, targetCol) <= COLOR_DIFFERENCE_THRESHOLD;
}

MapHandler::MapHandler(EntityHandler& entityHandler)
{
	// Load map texture
	this->map.loadFromFile("Resources/Maps/TestLevel.png");

	// Load entities and send them to the entityHandler
	this->loadEntitiesFromMap(entityHandler);
}

const sf::Texture& MapHandler::getMapTexture() const
{
	return this->map;
}