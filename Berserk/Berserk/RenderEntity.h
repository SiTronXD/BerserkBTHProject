#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"

class RenderEntity
{
private:
	const int NUM_ANIMATIONS_CAP = 4;

	sf::Vector3f position;
	sf::Vector2f worldScale;

	Animation* animations;

	int nrOfAnimations;
	int currentAnimationIndex;

	float tempTimer = 0.0f;

protected:
	void addAnimation(Animation animationToAdd);
	void setWorldScale(sf::Vector2f worldScale);

public:
	RenderEntity();
	virtual ~RenderEntity();

	void update(float dt);

	void setPosition(sf::Vector2f position);
	void setPosition(sf::Vector3f position);

	sf::IntRect getTextureIntRect() const;
	sf::Glsl::Vec4 getTextureRect() const;
	sf::Glsl::Vec3 getPosition() const;
	sf::Glsl::Vec2 getWorldScale() const;
};