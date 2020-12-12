#pragma once

#include <SFML/Graphics.hpp>
#include "RenderEntity.h"

class Collectible : public RenderEntity
{
private:
	const float COLLISION_RADIUS = 0.8f;

	sf::SoundBuffer foundSound;

	bool collectibleFound;

public:
	Collectible(sf::Vector2f position);

	void found();

	void update(float deltaTime) override;

	float getRadiusSqrd() const;
	bool hasBeenFound() const;
};