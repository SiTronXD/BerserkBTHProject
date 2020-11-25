#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
	sf::Font font;
	sf::Text text;

	bool isPressing;
	bool lastFrameIsPressing;
	bool isHovering;
	bool activate;

	void updateColor();

public:
	Button(float middleX, float middleY, std::string text);

	void update(sf::Vector2i mousePos, bool mouseBeingHeldDown);
	void render(sf::RenderWindow& window);

	bool hasBeenPressed();
};