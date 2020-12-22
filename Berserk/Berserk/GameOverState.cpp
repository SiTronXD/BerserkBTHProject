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

		this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");


		// Number of collectibles found text
		float foundCollectiblesPositionY = -70;
		this->foundCollectiblesText.setFont(this->font);
		this->foundCollectiblesText.setMainColor(TEXT_COLOR);
		this->foundCollectiblesText.setOutlineColor(OUTLINE_COLOR);
		this->foundCollectiblesText.setString("Found secrets: \n       (" + std::to_string(this->gameStats.getNumCollected()) + "/" + std::to_string(this->gameStats.getMaxNumCollectibles()) + ")");
		this->foundCollectiblesText.transformText(-670, foundCollectiblesPositionY, 63, 2);


		// Killed enemies text
		float killedEnemiesPositionX = -680;
		float killedEnemiesPositionY = -280;
		this->killedEnemiesText.setFont(this->font);
		this->killedEnemiesText.setMainColor(TEXT_COLOR);
		this->killedEnemiesText.setOutlineColor(OUTLINE_COLOR);
		this->killedEnemiesText.setString("Killed enemies:");
		this->killedEnemiesText.transformText(killedEnemiesPositionX - 2, killedEnemiesPositionY + 2, 40, 2);


		// Killed enemies number text
		this->killedEnemiesNumberText.setFont(this->font);
		this->killedEnemiesNumberText.setMainColor(TEXT_COLOR);
		this->killedEnemiesNumberText.setOutlineColor(OUTLINE_COLOR);
		this->killedEnemiesNumberText.setString("(" +
			std::to_string(this->gameStats.getNumKilledEnemies()) + "/" + std::to_string(this->gameStats.getMaxNumEnemies()) + ")"
		);
		this->killedEnemiesNumberText.transformText(killedEnemiesPositionX, killedEnemiesPositionY + 70, 40, 2);


		// Play time text
		float playTimeTextPositionY = -100;
		this->playTimeText.setFont(this->font);
		this->playTimeText.setMainColor(TEXT_COLOR);
		this->playTimeText.setOutlineColor(OUTLINE_COLOR);
		this->playTimeText.setString("Time:");
		this->playTimeText.transformText(650, playTimeTextPositionY, 40, 2);


		// Play time number text
		this->playTimeNumberText.setFont(this->font);
		this->playTimeNumberText.setMainColor(TEXT_COLOR);
		this->playTimeNumberText.setOutlineColor(OUTLINE_COLOR);
		this->playTimeNumberText.setString(
			std::to_string(this->gameStats.getPlayTimeInSeconds()) + " seconds"
		);
		this->playTimeNumberText.transformText(650, playTimeTextPositionY + 70, 40, 2);
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

	// Switch song depending on if the player won or not
	if(!this->gameStats.getPlayerHasWon())
		loadedMusic = this->gameOverMusic.openFromFile("Resources/Sounds/Music/waitingSoLong.ogg");
	else
		loadedMusic = this->gameOverMusic.openFromFile("Resources/Sounds/Music/behelit.ogg");

	// Apply settings to music
	if (loadedMusic && SettingsHandler::getMusicVolume() > 0)
	{
		this->gameOverMusic.setVolume(SettingsHandler::getMusicVolume());
		this->gameOverMusic.setLoop(false);
		this->gameOverMusic.play();
	}

	// Set start transition speed
	this->setStateTransitionSpeedScale(-1, 0.25f);
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
		this->foundCollectiblesText.draw(window);
		this->playTimeText.draw(window);
		this->playTimeNumberText.draw(window);
		this->killedEnemiesText.draw(window);
		this->killedEnemiesNumberText.draw(window);
	}
}
