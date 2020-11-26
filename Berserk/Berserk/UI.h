#pragma once

#include <SFML/Graphics.hpp>
#include "UITranslator.h"
#include <iostream>
#include <string>

class UI
{
private:
	sf::Font font;
	sf::Text fpsText;

	float updateDtTimer;

public:
	UI();

	void update(float deltaTime);

	void render(sf::RenderWindow& window);
};