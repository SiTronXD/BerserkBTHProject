#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

class RenderEntity
{
private:
	sf::Texture texture;
	sf::Vector3f position;

	Animation* animations;

public:
	RenderEntity();

	void setPosition(sf::Vector2f position);
	void setPosition(sf::Vector3f position);

	const sf::Texture& getTexture() const;
	sf::IntRect getTextureRect() const;
	sf::Glsl::Vec3 getPosition() const;
};