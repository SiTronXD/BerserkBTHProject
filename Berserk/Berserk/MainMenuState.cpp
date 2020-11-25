#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow& window)
	: GameState(window),
	playButton(400, 400, "PLAY"),
	exitButton(400, 600, "EXIT")
{
	this->backgroundTexture.loadFromFile("Resources/Textures/MainMenuBackgroundTexture.png");
	this->backgroundSprite.setTexture(this->backgroundTexture);

	float sizeFactor = SettingsHandler::getWindowSizeFactor(this->backgroundSprite.getGlobalBounds().height);
	this->backgroundSprite.setScale(sizeFactor, sizeFactor);
	this->backgroundSprite.setPosition(
		SettingsHandler::getWidth() / 2.0f - this->backgroundSprite.getGlobalBounds().width / 2.0f,
		SettingsHandler::getHeight() / 2.0f - this->backgroundSprite.getGlobalBounds().height / 2.0f
	);
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::handlePollEvent(const sf::Event& event)
{

}

void MainMenuState::update(float deltaTime)
{
	this->playButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	this->exitButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));

	// Start game
	if (this->playButton.hasBeenPressed())
	{
		GameState::setState(State::PLAY);
	}
	// Exit
	else if (this->exitButton.hasBeenPressed())
	{
		window.close();
	}
}

void MainMenuState::render()
{
	window.draw(this->backgroundSprite);

	this->playButton.render(window);
	this->exitButton.render(window);
}
