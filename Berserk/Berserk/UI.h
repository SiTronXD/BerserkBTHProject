#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "ResTranslator.h"
#include "CollisionHandler.h"

class UI
{
private:
	sf::Font font;
	sf::Text fpsText;
	sf::Text enterGoalText;

	CollisionHandler& collisionHandler;

	float updateDtTimer;

public:
	UI(CollisionHandler& collisionHandler);

	void update(float deltaTime);

	void render(sf::RenderWindow& window);
};