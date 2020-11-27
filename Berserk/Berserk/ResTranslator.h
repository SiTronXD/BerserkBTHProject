#pragma once

#include <SFML/Graphics.hpp>
#include "SettingsHandler.h"

class ResTranslator
{
private:
	static const unsigned int INTERNAL_WIDTH = 1920;
	static const unsigned int INTERNAL_HEIGHT = 1080;

public:
	static void transformSprite(sf::Sprite& sprite, float spriteMiddleX, float spriteMiddleY, 
		float spriteInternalWidth, float spriteInternalHeight);

	static void transformText(sf::Text& text, float textMiddleX, float textMiddleY,
		float textCharacterSize);
};