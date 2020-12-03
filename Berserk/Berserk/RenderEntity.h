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

	bool shouldRemove;

protected:
	void addAnimation(Animation animationToAdd);
	void setWorldScale(sf::Vector2f worldScale);

public:
	RenderEntity();
	virtual ~RenderEntity();

	virtual void update(float dt);

	void setPosition(sf::Vector2f position);
	void setPosition(sf::Vector3f position);
	void flagShouldRemove();

	sf::IntRect getTextureIntRect() const;
	sf::Glsl::Vec4 getTextureRectGlsl() const;
	sf::Glsl::Vec3 getPositionGlsl() const;
	sf::Glsl::Vec2 getWorldScaleGlsl() const;

	sf::Vector2f getPosition2D() const;
	sf::Vector3f getPosition3D() const;

	const bool getShouldRemove() const;
};