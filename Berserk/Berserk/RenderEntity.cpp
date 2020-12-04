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
	: animations(nullptr), nrOfAnimations(0), currentAnimationIndex(0), worldScale(1, 1),
	shouldRemove(false)
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
}

void RenderEntity::setPosition(sf::Vector2f position)
{
	this->position = sf::Vector3f(position.x, position.y, this->position.z);
}

void RenderEntity::setPosition(sf::Vector3f position)
{
	this->position = position;
}

void RenderEntity::flagShouldRemove()
{
	this->shouldRemove = true;
}

void RenderEntity::setAnimationIndex(int newIndex)
{
	this->currentAnimationIndex = newIndex;
}

void RenderEntity::resetCurrentAnimation()
{
	this->animations[this->currentAnimationIndex].reset();
}

int RenderEntity::getCurrentAnimationIndex() const
{
	return this->currentAnimationIndex;
}

int RenderEntity::getNrOfAnimations() const
{
	return this->nrOfAnimations;
}

const Animation& RenderEntity::getCurrentAnimation() const
{
	return this->animations[this->currentAnimationIndex];
}

sf::IntRect RenderEntity::getTextureIntRect() const
{
	return this->animations[this->currentAnimationIndex].getCurrentRect();
}

sf::Glsl::Vec4 RenderEntity::getTextureRectGlsl() const
{
	sf::IntRect intRect = this->animations[this->currentAnimationIndex].getCurrentRect();

	return sf::Glsl::Vec4(intRect.left, intRect.top, intRect.width, intRect.height);
}

sf::Glsl::Vec3 RenderEntity::getPositionGlsl() const
{
	return sf::Glsl::Vec3(this->position.x, this->position.y, this->position.z);
}

sf::Glsl::Vec2 RenderEntity::getWorldScaleGlsl() const
{
	return sf::Glsl::Vec2(this->worldScale.x, this->worldScale.y);
}

sf::Vector2f RenderEntity::getPosition2D() const
{
	return sf::Vector2f(this->position.x, this->position.y);
}

sf::Vector3f RenderEntity::getPosition3D() const
{
	return this->position;
}

bool RenderEntity::getShouldRemove() const
{
	return this->shouldRemove;
}