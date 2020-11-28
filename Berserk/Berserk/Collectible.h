#pragma once

#include <SFML/Graphics.hpp>
#include "RenderEntity.h"

class Collectible : public RenderEntity
{
private:

public:
	Collectible(sf::Vector2f position);
};