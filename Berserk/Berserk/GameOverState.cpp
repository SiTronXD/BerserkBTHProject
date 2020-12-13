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

		const sf::Color BUTTON_TEXT_COLOR = sf::Color(61, 170, 36);

		// Set text color for buttons
		this->restartButton.setTextColor(BUTTON_TEXT_COLOR);
		this->mainMenuButton.setTextColor(BUTTON_TEXT_COLOR);

		// Move buttons
		this->restartButton.set(-690, 100);
		this->mainMenuButton.set(650, 100);

		// Text
		const sf::Color TEXT_COLOR = sf::Color(80, 190, 50);
		const sf::Color OUTLINE_COLOR = sf::Color(200, 255, 200);

		// Stats text
		this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");
		this->foundCollectiblesText.setFont(this->font);
		this->foundCollectiblesText.setFillColor(TEXT_COLOR);
		this->foundCollectiblesText.setString("Found collectibles: \n          (" + 
			std::to_string(this->gameStats.getNumCollected()) + "/" + std::to_string(this->gameStats.getMaxNumCollectibles()) + ")");
		ResTranslator::transformText(this->foundCollectiblesText, -670, -150, 60);

		this->foundCollectiblesOutlineText = foundCollectiblesText;
		this->foundCollectiblesOutlineText.setFillColor(OUTLINE_COLOR);
		ResTranslator::transformText(this->foundCollectiblesOutlineText, -672, -148, 60);

		this->playTimeText.setFont(this->font);
		this->playTimeText.setFillColor(TEXT_COLOR);
		this->playTimeText.setString("   Time: \n" + std::to_string(this->gameStats.getPlayTimeInSeconds()) + " seconds");
		ResTranslator::transformText(this->playTimeText, 650, -150, 60);

		this->playTimeOutlineText = playTimeText;
		this->playTimeOutlineText.setFillColor(OUTLINE_COLOR);
		ResTranslator::transformText(this->playTimeOutlineText, 648, -148, 60);
	}
	else
	{
		backgroundTexture.loadFromFile("Resources/Textures/YouLostBackgroundTexture.png");

		// Move buttons
		this->restartButton.set(-550, 30);
		this->mainMenuButton.set(-550, 250);
	}

	// Background sprite
	this->backgroundSprite.setTexture(this->backgroundTexture);
	ResTranslator::transformSprite(this->backgroundSprite, 0, 0, 2520, 1080);

	// Music
	bool loadedMusic = true;

	if(!this->gameStats.getPlayerHasWon())
		loadedMusic = this->gameOverMusic.openFromFile("Resources/Sounds/Music/waitingSoLong.ogg");
	else
		loadedMusic = this->gameOverMusic.openFromFile("Resources/Sounds/Music/behelit.ogg");

	if (loadedMusic && SettingsHandler::getMusicVolume() > 0)
	{
		this->gameOverMusic.setVolume(SettingsHandler::getMusicVolume());
		this->gameOverMusic.setLoop(false);
		this->gameOverMusic.play();
	}

	// Set start transition speed
	this->setStateTransitionSpeedScale(-1.0f, 0.25f);
}

void GameOverState::update(float deltaTime)
{
	this->restartButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	this->mainMenuButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));

	// Restart game
	if (this->restartButton.hasBeenPressed())
	{
		// Stop music
		this->gameOverMusic.stop();

		GameState::setState(State::PLAY);
	}
	// Go to main menu
	else if (this->mainMenuButton.hasBeenPressed())
	{
		// Stop music
		this->gameOverMusic.stop();

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

	// Collectibles and play time texts
	if (gameStats.getPlayerHasWon())
	{
		window.draw(this->foundCollectiblesOutlineText);
		window.draw(this->foundCollectiblesText);
		window.draw(this->playTimeOutlineText);
		window.draw(this->playTimeText);
	}
}
