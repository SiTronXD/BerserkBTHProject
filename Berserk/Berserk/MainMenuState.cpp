#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow& window)
	: GameState(window),
	playButton(-650, -100, "PLAY"),
	exitButton(-650,  150, "EXIT")
{
	//this->backgroundTexture.loadFromFile("Resources/Textures/MainMenuBackgroundTextureAlignment.png");
	this->backgroundTexture.loadFromFile("Resources/Textures/MainMenuBackgroundTexture.png");
	this->backgroundSprite.setTexture(this->backgroundTexture);

	UITranslator::translateSprite(this->backgroundSprite, 0, 0, 2520, 1080);
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
