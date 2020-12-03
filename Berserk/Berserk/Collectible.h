#pragma once

#include <SFML/Graphics.hpp>
#include "RenderEntity.h"

class Collectible : public RenderEntity
{
private:
	const float COLLISION_RADIUS = 0.8f;

public:
	Collectible(sf::Vector2f position);

	float getRadiusSqrd() const;
};