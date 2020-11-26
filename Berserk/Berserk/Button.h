#pragma once

#include <SFML/Graphics.hpp>
#include "UITranslator.h"

class Button
{
private:
	sf::Font font;
	sf::Text text;
	sf::Text outlineText;

	float middleX;
	float middleY;
	float offsetSize;
	float characterSize;

	bool isPressing;
	bool lastFrameIsPressing;
	bool isHovering;
	bool activate;

	void updateLook();

public:
	Button(float middleX, float middleY, std::string text);

	void update(sf::Vector2i mousePos, bool mouseBeingHeldDown);
	void render(sf::RenderWindow& window);

	bool hasBeenPressed();
};