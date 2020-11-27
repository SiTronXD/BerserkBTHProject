#include "MapHandler.h"

void MapHandler::loadEntitiesFromMap(EntityHandler& entityHandler)
{
	const sf::Color GOAL_COLOR = sf::Color(0, 255, 0, 255);

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
			
			// Goal
			sf::Color goalTarget = pixelColor - GOAL_COLOR;
			if (SMath::dot(goalTarget, goalTarget) <= COLOR_DIFFERENCE_THRESHOLD)
			{
				entityHandler.placeGoal(sf::Vector2f(x - 0.5f, mapHeight + 1 - (y - 0.5f)));
			}
		}
	}
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