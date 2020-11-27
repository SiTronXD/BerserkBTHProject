#pragma once

#include <SFML/Graphics.hpp>
#include "ResTranslator.h"

class Button
{
private:
	static const int DEFAULT_CHARACTER_SIZE = 45;

	sf::Font font;
	sf::Text text;
	sf::Text outlineText;

	sf::Color defaultColor;

	std::string buttonText;

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
	
	void setTextColor(sf::Color newColor);
	void set(float middleX, float middleY);
	void set(float middleX, float middleY, std::string text);

	bool hasBeenPressed();
};