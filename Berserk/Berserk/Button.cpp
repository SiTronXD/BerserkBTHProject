#include "Button.h"

void Button::updateLook()
{
	if (this->isPressing)
	{
		this->text.setOutlineColor(sf::Color::Yellow);
	}
	else
	{
		this->text.setOutlineColor(sf::Color::White);

		if (this->isHovering)
			this->text.setMainColor(sf::Color::White);
		else
			this->text.setMainColor(defaultColor);
	}
}

Button::Button(float middleX, float middleY, std::string text, bool playSoundWhenActivated)
	: middleX(middleX), middleY(middleY), isPressing(false), 
	lastFrameIsPressing(false), isHovering(false), activate(false),
	offsetSize(4), characterSize(DEFAULT_CHARACTER_SIZE), 
	defaultColor(sf::Color::Red), buttonText(text), playSoundWhenActivated(playSoundWhenActivated),
	text(middleX, middleY, characterSize)
{
	// Text
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");
	this->text.setFont(this->font);
	this->text.setMainColor(sf::Color::Red);
	this->text.setOutlineColor(sf::Color::White);

	// Sound
	this->soundPlayer.setVolume(SettingsHandler::getSoundEffectsVolume());
	this->buttonActivatedSound.loadFromFile("Resources/Sounds/buttonActivated.wav");

	this->set(middleX, middleY, text);
}

void Button::update(sf::Vector2i mousePos, bool mouseBeingHeldDown)
{
	// Mouse is within bounds
	this->isHovering = this->text.getBounds().contains((sf::Vector2f)mousePos);
	if (this->isHovering)
	{
		this->isPressing = mouseBeingHeldDown;

		// Just released mouse button
		if (!this->isPressing && this->lastFrameIsPressing && !this->activate)
		{
			this->activate = true;

			// Sound
			if (this->playSoundWhenActivated)
			{
				this->soundPlayer.setBuffer(this->buttonActivatedSound);
				this->soundPlayer.play();
			}
		}
	}
	else
		this->isPressing = false;

	// Update visual appearance
	this->updateLook();

	this->lastFrameIsPressing = this->isPressing;
}

void Button::render(sf::RenderWindow& window)
{
	this->text.drawOutlineText(window);

	// Draw main text, only if the button is not being pressed
	if (!this->isPressing)
		this->text.drawMainText(window);
}

// Sets the main text to a new color
void Button::setTextColor(sf::Color newColor)
{
	defaultColor = newColor;
}

void Button::set(float middleX, float middleY)
{
	this->set(middleX, middleY, this->buttonText);
}

void Button::set(float middleX, float middleY, std::string text)
{
	this->characterSize = DEFAULT_CHARACTER_SIZE;

	// Set text strings
	this->text.setString(text);

	// Move
	this->text.transformText(middleX, middleY, this->characterSize);
}

bool Button::hasBeenPressed()
{
	return activate;
}
