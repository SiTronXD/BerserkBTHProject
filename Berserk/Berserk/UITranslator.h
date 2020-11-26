#pragma once

#include <SFML/Graphics.hpp>
#include "SettingsHandler.h"

class UITranslator
{
private:
	static const unsigned int INTERNAL_WIDTH = 1920;
	static const unsigned int INTERNAL_HEIGHT = 1080;

public:
	static void translateSprite(sf::Sprite& sprite, float spriteMiddleX, float spriteMiddleY, 
		float spriteInternalWidth, float spriteInternalHeight);

	static void translateText(sf::Text& text, float textMiddleX, float textMiddleY,
		float textCharacterSize);
};