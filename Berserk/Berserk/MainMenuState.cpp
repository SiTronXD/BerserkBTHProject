#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow& window)
	: GameState(window),
	playButton(-650, -100, "PLAY"),
	exitButton(-650,  150, "EXIT", false)
{
	//this->backgroundTexture.loadFromFile("Resources/Textures/MainMenuBackgroundTextureAlignment.png");
	this->backgroundTexture.loadFromFile("Resources/Textures/MainMenuBackgroundTexture.png");
	this->backgroundSprite.setTexture(this->backgroundTexture);

	ResTranslator::transformSprite(this->backgroundSprite, 0, 0, 2520, 1080);

	// Music
	if (this->menuMusic.openFromFile("Resources/Sounds/Music/bloodAndGuts.ogg") &&
		SettingsHandler::getMusicVolume() > 0)
	{
		this->menuMusic.setVolume(SettingsHandler::getMusicVolume());
		this->menuMusic.setLoop(true);
		this->menuMusic.play();
	}
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::update(float deltaTime)
{
	this->playButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	this->exitButton.update(sf::Mouse::getPosition(this->window), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	
	// Start game
	if (this->playButton.hasBeenPressed())
	{
		// Sound
		this->menuMusic.stop();

		GameState::setState(State::PLAY);
	}
	// Exit
	else if (this->exitButton.hasBeenPressed())
	{
		// Sound
		this->menuMusic.stop();

		window.close();
	}
}

void MainMenuState::render()
{
	window.draw(this->backgroundSprite);

	this->playButton.render(window);
	this->exitButton.render(window);
}
