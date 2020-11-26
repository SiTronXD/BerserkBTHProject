#pragma once

#include <SFML/Graphics.hpp>
#include "EntityHandler.h"

class MapHandler
{
private:
	const float COLOR_DIFFERENCE_THRESHOLD = 0.05f;

	sf::Texture map;

	float colorDot(sf::Color& c);

	void loadEntitiesFromMap(EntityHandler& entityHandler);

public:
	MapHandler(EntityHandler& entityHandler);

	const sf::Texture& getMapTexture() const;
};
