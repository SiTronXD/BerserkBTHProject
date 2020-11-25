#include "Button.h"

void Button::updateColor()
{
	if (this->isPressing)
		this->text.setFillColor(sf::Color::Yellow);
	else if (this->isHovering)
		this->text.setFillColor(sf::Color::White);
	else
		this->text.setFillColor(sf::Color::Red);
}

Button::Button(float middleX, float middleY, std::string text)
	: isPressing(false), lastFrameIsPressing(false), isHovering(false), activate(false)
{
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");
	this->text.setFont(this->font);
	this->text.setPosition(middleX, middleY);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Red);
}

void Button::update(sf::Vector2i mousePos, bool mouseBeingHeldDown)
{
	// Mouse is within bounds
	this->isHovering = this->text.getGlobalBounds().contains((sf::Vector2f)mousePos);
	if (this->isHovering)
	{
		this->isPressing = mouseBeingHeldDown;

		// Just released mouse button
		if (!this->isPressing && this->lastFrameIsPressing)
		{
			this->activate = true;
		}
	}
	else
		this->isPressing = false;

	this->updateColor();

	this->lastFrameIsPressing = this->isPressing;
}

void Button::render(sf::RenderWindow& window)
{
	window.draw(text);
}

bool Button::hasBeenPressed()
{
	return activate;
}
