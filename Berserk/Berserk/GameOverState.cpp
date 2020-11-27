#include "GameOverState.h"


GameOverState::GameOverState(sf::RenderWindow& window, GameStatsHandler& gameStats)
	: GameState(window), restartButton(0, 0, "RESTART"), mainMenuButton(0, 0, "MAIN MENU"), 
	gameStats(gameStats)
{
	// Show mouse again
	this->window.setMouseCursorVisible(true);

	// Player has won
	if (gameStats.getPlayerHasWon())
	{
		backgroundTexture.loadFromFile("Resources/Textures/YouWonBackgroundTexture.png");

		// Set text color for buttons
		this->restartButton.setTextColor(sf::Color(61, 170, 36));
		this->mainMenuButton.setTextColor(sf::Color(61, 170, 36));

		// Move buttons
		this->restartButton.set(-690, 0);
		this->mainMenuButton.set(650, 0);
	}
	else
	{
		backgroundTexture.loadFromFile("Resources/Textures/YouLostBackgroundTexture.png");

		// Move buttons
		this->restartButton.set(-550, 30);
		this->mainMenuButton.set(-550, 250);
	}

	this->backgroundSprite.setTexture(this->backgroundTexture);
	ResTranslator::transformSprite(this->backgroundSprite, 0, 0, 2520, 1080);
}

void GameOverState::handlePollEvent(const sf::Event& event)
{
}

void GameOverState::update(float deltaTime)
{
	this->restartButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	this->mainMenuButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));

	// Restart game
	if (this->restartButton.hasBeenPressed())
	{
		GameState::setState(State::PLAY);
	}
	// Go to main menu
	else if (this->mainMenuButton.hasBeenPressed())
	{
		GameState::setState(State::MAIN_MENU);
	}
}

void GameOverState::render()
{
	// Background
	window.draw(this->backgroundSprite);

	// Buttons
	this->restartButton.render(this->window);
	this->mainMenuButton.render(this->window);
}
