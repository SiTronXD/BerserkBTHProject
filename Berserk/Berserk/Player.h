#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "SettingsHandler.h"
#include "SMath.h"

class Player
{
private:
	const float MOVEMENT_SPEED = 4.0f;

	float x;
	float y;
	float direction; // Direction is in radians

	float lastMouseX;

public:
	Player(int x, int y);

	void handleInput(float deltaTime);

	const sf::Glsl::Vec3 getPlayerCamera() const;
};