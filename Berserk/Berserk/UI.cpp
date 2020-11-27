#include "UI.h"

UI::UI(CollisionHandler& collisionHandler)
	: updateDtTimer(1.0f), collisionHandler(collisionHandler)
{
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");

	// Frames per second text
	this->fpsText.setFont(this->font);
	this->fpsText.setFillColor(sf::Color::Red);
	this->fpsText.setString("FPS: ");
	ResTranslator::transformText(this->fpsText, -850, -500, 40);

	// Press [F] to exit text
	this->enterGoalText.setFont(this->font);
	this->enterGoalText.setFillColor(sf::Color::Red);
	this->enterGoalText.setString("PRESS [F] TO EXIT");
	ResTranslator::transformText(this->enterGoalText, 0, -100, 40);
}

void UI::update(float deltaTime)
{
	this->updateDtTimer += deltaTime;

	// Update frames per second text
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

	// Show exit text if necessary
	if (collisionHandler.playerIsCloseToGoal())
		window.draw(this->enterGoalText);
}
