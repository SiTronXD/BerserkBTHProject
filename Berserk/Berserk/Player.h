#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "ResTranslator.h"
#include "SettingsHandler.h"
#include "SMath.h"

class Player
{
private:
	const float MOVEMENT_SPEED = 6.0f;
	const float SWORD_SPRITE_SCALE = 10.0f;

	sf::Vector2i monitorMiddle;

	sf::Texture swordTextureSheet;
	sf::Sprite swordSprite;

	float x;
	float y;
	float direction; // Direction is in radians
	float walkTimer;

	sf::Vector2f swordPosition;

	bool tryToExit;

	void updateSwordPosition();

public:
	Player(int x, int y);

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);

	const bool playerTriesToExit() const;
	const sf::Vector2f getPlayerPosition() const;
	const sf::Glsl::Vec3 getPlayerCamera() const;
};