#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "SettingsHandler.h"
#include "SMath.h"

class Player
{
private:
	const float MOVEMENT_SPEED = 4.0f;

	sf::Vector2i monitorMiddle;

	float x;
	float y;
	float direction; // Direction is in radians

	bool tryToExit;

public:
	Player(int x, int y);

	void handleInput(float deltaTime);

	const bool playerTriesToExit() const;
	const sf::Vector2f getPlayerPosition() const;
	const sf::Glsl::Vec3 getPlayerCamera() const;
};