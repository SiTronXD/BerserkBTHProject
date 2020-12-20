#pragma once

#include <SFML/Graphics.hpp>
#include "ResTranslator.h"

class OutlineText
{
private:
	sf::Text text;
	sf::Text outlineText;

public:
	OutlineText();
	OutlineText(float x, float y, float characterSize);

	void setFont(sf::Font& font);
	void setString(std::string text);
	void setMainColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void transformText(float middleX, float middleY, float characterSize, int outlineOffset = 4);

	void draw(sf::RenderWindow& window);
	void drawMainText(sf::RenderWindow& window);
	void drawOutlineText(sf::RenderWindow& window);

	sf::FloatRect getBounds() const;
};