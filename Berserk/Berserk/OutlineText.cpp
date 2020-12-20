#include "OutlineText.h"

OutlineText::OutlineText()
{ }

OutlineText::OutlineText(float x, float y, float characterSize)
{
	this->transformText(x, y, characterSize);
}

void OutlineText::setFont(sf::Font& font)
{
	this->text.setFont(font);
	this->outlineText.setFont(font);
}

void OutlineText::setString(std::string text)
{
	this->text.setString(text);
	this->outlineText.setString(text);
}

void OutlineText::setMainColor(sf::Color color)
{
	this->text.setFillColor(color);
}

void OutlineText::setOutlineColor(sf::Color color)
{
	this->outlineText.setFillColor(color);
}

void OutlineText::transformText(float middleX, float middleY, float characterSize, int outlineOffset)
{
	ResTranslator::transformText(
		this->text, 
		middleX, 
		middleY, 
		characterSize
	);
	ResTranslator::transformText(
		this->outlineText, 
		middleX - outlineOffset, 
		middleY + outlineOffset, 
		characterSize
	);
}

void OutlineText::draw(sf::RenderWindow& window)
{
	this->drawOutlineText(window);
	this->drawMainText(window);
}

void OutlineText::drawMainText(sf::RenderWindow& window)
{
	window.draw(this->text);
}

void OutlineText::drawOutlineText(sf::RenderWindow& window)
{
	window.draw(this->outlineText);
}

sf::FloatRect OutlineText::getBounds() const
{
	return this->text.getGlobalBounds();
}
