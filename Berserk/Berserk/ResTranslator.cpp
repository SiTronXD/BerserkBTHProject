#include "ResTranslator.h"

void ResTranslator::transformSprite(sf::Sprite& sprite, float spriteMiddleX, float spriteMiddleY,
	float spriteInternalWidth, float spriteInternalHeight)
{
	float screenMidX = SettingsHandler::getWidth() / 2.0f;
	float screenMidY = SettingsHandler::getHeight() / 2.0f;

	float screenSizeScaleY = (float) SettingsHandler::getHeight() / INTERNAL_HEIGHT;

	// Apply transformation
	sprite.setScale(
		(spriteInternalWidth / sprite.getGlobalBounds().width) * screenSizeScaleY,
		(spriteInternalHeight / sprite.getGlobalBounds().height) * screenSizeScaleY
	);
	sprite.setPosition(
		(float)(screenMidX + (spriteMiddleX * screenSizeScaleY) - sprite.getGlobalBounds().width / 2.0f),
		(float)(screenMidY + (spriteMiddleY * screenSizeScaleY) - sprite.getGlobalBounds().height / 2.0f)
	);
}

void ResTranslator::transformText(sf::Text& text, float textMiddleX, float textMiddleY,
	float textCharacterSize)
{
	float screenMidX = SettingsHandler::getWidth() / 2.0f;
	float screenMidY = SettingsHandler::getHeight() / 2.0f;

	float screenSizeScaleY = (float)SettingsHandler::getHeight() / INTERNAL_HEIGHT;

	// Apply transformation
	text.setCharacterSize(30);	// Set to default first
	text.setCharacterSize(
		(int) (textCharacterSize * (textCharacterSize / text.getGlobalBounds().height) * screenSizeScaleY)
	);
	text.setPosition(
		(float)(screenMidX + (textMiddleX * screenSizeScaleY) - text.getGlobalBounds().width / 2.0f),
		(float)(screenMidY + (textMiddleY * screenSizeScaleY) - text.getGlobalBounds().height / 2.0f)
	);
}