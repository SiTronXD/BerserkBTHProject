#pragma once

#include <SFML/Graphics.hpp>
#include "EntityHandler.h"
#include "SMath.h"

class MapHandler
{
private:
	const float COLOR_DIFFERENCE_THRESHOLD = 0.005f;

	sf::Texture map;

	void loadEntitiesFromMap(EntityHandler& entityHandler);

	bool evaluatePixel(sf::Color currentPixel, sf::Color targetPixel);

public:
	MapHandler(EntityHandler& entityHandler);

	const sf::Texture& getMapTexture() const;
};
