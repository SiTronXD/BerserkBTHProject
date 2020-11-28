#include "RenderEntity.h"

RenderEntity::RenderEntity()
	: animations(nullptr)
{
	texture.loadFromFile("Resources/Textures/RenderEntityTextureSheet.png");
}

void RenderEntity::setPosition(sf::Vector2f position)
{
	this->position = sf::Vector3f(position.x, position.y, this->position.z);
}

void RenderEntity::setPosition(sf::Vector3f position)
{
	this->position = position;
}

const sf::Texture& RenderEntity::getTexture() const
{
	return this->texture;
}

sf::IntRect RenderEntity::getTextureRect() const
{
	return sf::IntRect(0, 0, 64, 64);
}

sf::Glsl::Vec3 RenderEntity::getPosition() const
{
	return sf::Glsl::Vec3(position.x, position.y, position.z);
}
