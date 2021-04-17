#pragma once

#include <SFML/Graphics.hpp>
#include "EntityHandler.h"
#include "../../Engine/SMath.h"

class MapHandler
{
private:
	const float COLOR_DIFFERENCE_THRESHOLD = 0.005f;

	sf::Texture map;

	void loadFromMap(EntityHandler& entityHandler);

	bool evaluatePixel(sf::Color currentPixel, sf::Color targetPixel);

	MapHandler(const MapHandler& other) = delete;
	MapHandler operator=(const MapHandler& other) = delete;

public:
	MapHandler(EntityHandler& entityHandler);

	const sf::Texture& getMapTexture() const;
};
