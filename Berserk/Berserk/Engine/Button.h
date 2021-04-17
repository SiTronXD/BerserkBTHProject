#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResTranslator.h"
#include "OutlineText.h"

class Button
{
private:
	static const int DEFAULT_CHARACTER_SIZE = 45;

	sf::Font font;
	OutlineText text;

	sf::Sound soundPlayer;
	sf::SoundBuffer buttonActivatedSound;

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
	bool playSoundWhenActivated;

	void updateLook();

public:
	Button(float middleX, float middleY, std::string text, bool playSoundWhenActivated = true);

	void update(sf::Vector2i mousePos, bool mouseBeingHeldDown);
	void render(sf::RenderWindow& window);
	
	void setTextColor(sf::Color newColor);
	void set(float middleX, float middleY);
	void set(float middleX, float middleY, std::string text);

	bool hasBeenPressed();
};