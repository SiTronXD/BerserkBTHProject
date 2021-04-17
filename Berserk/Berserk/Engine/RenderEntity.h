#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "SettingsHandler.h"

class RenderEntity
{
private:
	const int NUM_ANIMATIONS_CAP = 4;

	sf::Vector3f position;
	sf::Vector2f worldScale;

	Animation* animations;

	sf::Sound soundPlayer;

	int nrOfAnimations;
	int currentAnimationIndex;

	bool shouldRemove;

	void cleanUpMemory();

	RenderEntity(const RenderEntity& other) = delete;
	RenderEntity operator=(const RenderEntity& other) = delete;

protected:
	void addAnimation(Animation& animationToAdd);
	void setWorldScale(sf::Vector2f worldScale);
	void setAnimationIndex(int newIndex);
	void resetCurrentAnimation();
	void playSound(sf::SoundBuffer& sound, float volumeScale = 1.0f, float pitch = 1.0f);

	int getCurrentAnimationIndex() const;
	int getNrOfAnimations() const;

	const Animation& getCurrentAnimation() const;

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
	sf::Vector2f getWorldScale() const;

	bool getShouldRemove() const;
};