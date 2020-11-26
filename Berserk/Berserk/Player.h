#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:
	float x;
	float y;
	float direction; // Direction is in radians

public:
	Player(int x, int y);

	void handleInput(float deltaTime);
	void addDirection(float deltaDirection);

	const sf::Glsl::Vec3 getPlayerCamera() const;
};