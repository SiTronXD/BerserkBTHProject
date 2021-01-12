#include "MapHandler.h"

void MapHandler::loadFromMap(EntityHandler& entityHandler)
{
	const sf::Color WALL_COLOR = sf::Color(255, 255, 255, 255);
	const sf::Color GOAL_COLOR = sf::Color(0, 255, 0, 255);
	const sf::Color COLLECTIBLE_COLOR = sf::Color(255, 255, 0, 255);
	const sf::Color SINGLE_ENEMY_COLOR = sf::Color(255, 0, 0, 255);
	const sf::Color PLAYER_SPAWN_POINT_COLOR = sf::Color(0, 0, 255, 255);

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
				entityHandler.placeWall(sf::Vector2i(x, y));

			// Goal
			if (this->evaluatePixel(pixelColor, GOAL_COLOR))
			{
				entityHandler.placeGoal(sf::Vector2f(x + 0.5f, y + 0.5f));

				// Set wall
				entityHandler.placeWall(sf::Vector2i(x, y));
			}

			// Collectible
			if (this->evaluatePixel(pixelColor, COLLECTIBLE_COLOR))
				entityHandler.addCollectible(sf::Vector2f(x + 0.5f, y + 0.5f));

			// Single enemy
			if (this->evaluatePixel(pixelColor, SINGLE_ENEMY_COLOR))
				entityHandler.addEnemy(sf::Vector2f(x + 0.5f, y + 0.5f));

			// Player spawn point
			if (this->evaluatePixel(pixelColor, PLAYER_SPAWN_POINT_COLOR))
				entityHandler.placePlayer(sf::Vector2f(x + 0.5f, y + 0.5f));
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
	this->map.loadFromFile("Resources/Maps/MainLevel.png");
	//this->map.loadFromFile("Resources/Maps/BenchmarkLevel.png");

	// Load entities and send them to the entityHandler
	this->loadFromMap(entityHandler);
}

const sf::Texture& MapHandler::getMapTexture() const
{
	return this->map;
}