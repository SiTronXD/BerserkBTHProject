#include "RenderEntity.h"

void RenderEntity::addAnimation(Animation animationToAdd)
{
	if(this->nrOfAnimations < this->NUM_ANIMATIONS_CAP - 1)
		this->animations[this->nrOfAnimations++] = animationToAdd;
}

void RenderEntity::setWorldScale(sf::Vector2f worldScale)
{
	this->worldScale = worldScale;
}

RenderEntity::RenderEntity()
	: animations(nullptr), nrOfAnimations(0), currentAnimationIndex(0), worldScale(1, 1)
{
	this->animations = new Animation[NUM_ANIMATIONS_CAP] { };
}

RenderEntity::~RenderEntity()
{
	delete[] this->animations;
}

void RenderEntity::update(float dt)
{
	this->animations[this->currentAnimationIndex].update(dt);

	// Demonstrate scale and position
	this->tempTimer += dt;
	float s = std::sin(tempTimer) * 0.5f + 0.5f;
	this->worldScale = sf::Vector2f(s, s);
	this->position.z = std::sin(tempTimer * 2.0f);
}

void RenderEntity::setPosition(sf::Vector2f position)
{
	this->position = sf::Vector3f(position.x, position.y, this->position.z);
}

void RenderEntity::setPosition(sf::Vector3f position)
{
	this->position = position;
}

sf::IntRect RenderEntity::getTextureIntRect() const
{
	return this->animations[this->currentAnimationIndex].getCurrentRect();
}

sf::Glsl::Vec4 RenderEntity::getTextureRect() const
{
	sf::IntRect intRect = this->animations[this->currentAnimationIndex].getCurrentRect();

	return sf::Glsl::Vec4(intRect.left, intRect.top, intRect.width, intRect.height);
}

sf::Glsl::Vec3 RenderEntity::getPosition() const
{
	return sf::Glsl::Vec3(this->position.x, this->position.y, this->position.z);
}

sf::Glsl::Vec2 RenderEntity::getWorldScale() const
{
	return sf::Glsl::Vec2(this->worldScale.x, this->worldScale.y);
}
