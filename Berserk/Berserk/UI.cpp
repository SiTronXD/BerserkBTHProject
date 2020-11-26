#include "UI.h"

UI::UI()
	: updateDtTimer(1.0f)
{
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");
	this->fpsText.setFont(this->font);
	this->fpsText.setFillColor(sf::Color::Red);
	this->fpsText.setString("FPS: ");

	UITranslator::transformText(this->fpsText, -850, -500, 40);
}

void UI::update(float deltaTime)
{
	this->updateDtTimer += deltaTime;

	// Update text
	if (this->updateDtTimer >= 0.25f)
	{
		std::string fpsText = "FPS: " + std::to_string((int)(1.0f / deltaTime));

		this->fpsText.setString(fpsText);

		this->updateDtTimer = 0.0f;
	}
}

void UI::render(sf::RenderWindow& window)
{
	window.draw(this->fpsText);
}
