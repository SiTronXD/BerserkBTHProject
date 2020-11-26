#include "MapHandler.h"


float MapHandler::colorDot(sf::Color& c)
{
	float r = c.r / 255.0f;
	float g = c.g / 255.0f;
	float b = c.b / 255.0f;

	return r*r + g*g + b*b;
}

void MapHandler::loadEntitiesFromMap(EntityHandler& entityHandler)
{
	const sf::Color GOAL_COLOR = sf::Color(0, 255, 0, 255);

	// Go through each pixel in the map
	sf::Image image(this->map.copyToImage());
	for (int y = 0; y < image.getSize().y; y++)
	{
		for (int x = 0; x < image.getSize().x; x++)
		{
			sf::Color pixelColor = image.getPixel(x, y);
			
			// Goal
			sf::Color goalTarget = pixelColor - GOAL_COLOR;
			if (colorDot(goalTarget) <= COLOR_DIFFERENCE_THRESHOLD)
			{
				entityHandler.placeGoal(sf::Vector2f(x, y));
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