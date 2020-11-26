#include "Button.h"

void Button::updateLook()
{
	if (this->isPressing)
	{
		this->outlineText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		this->outlineText.setFillColor(sf::Color::White);

		if (this->isHovering)
			this->text.setFillColor(sf::Color::White);
		else
			this->text.setFillColor(sf::Color::Red);
	}
}

Button::Button(float middleX, float middleY, std::string text)
	: middleX(middleX), middleY(middleY), isPressing(false), 
	lastFrameIsPressing(false), isHovering(false), activate(false),
	offsetSize(4), characterSize(50)
{
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");
	this->text.setFont(this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Red);
	this->outlineText.setFont(this->font);
	this->outlineText.setString(text);
	this->outlineText.setFillColor(sf::Color::White);

	UITranslator::transformText(this->text, middleX, middleY, this->characterSize);
	UITranslator::transformText(this->outlineText, middleX - this->offsetSize, middleY + this->offsetSize, this->characterSize);
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

	this->updateLook();

	this->lastFrameIsPressing = this->isPressing;
}

void Button::render(sf::RenderWindow& window)
{
	window.draw(this->outlineText);

	if(!this->isPressing)
		window.draw(this->text);
}

bool Button::hasBeenPressed()
{
	return activate;
}
