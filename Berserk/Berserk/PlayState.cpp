#include <iostream>
#include "PlayState.h"

PlayState::PlayState(sf::RenderWindow& window, GameStatsHandler& gameStats)
	: GameState(window), entityHandler(gameStats), mapHandler(entityHandler), renderer(mapHandler, entityHandler), 
	ui(entityHandler), gameStats(gameStats)
{
	// Reset and set stats
	this->gameStats.reset();
	this->gameStats.setMaxCollectibles(this->entityHandler.getNumCollectibles());
	this->gameStats.setMaxNumEnemies(this->entityHandler.getNumEnemies());

	// Set screen rectangle shape
	screenRenderRect.setSize(sf::Vector2f((float) SettingsHandler::getWidth(), (float) SettingsHandler::getHeight()));
	screenRenderRect.setFillColor(sf::Color::Green);

	// Load post processing shader
	if (!postProcessingShader.loadFromFile("Resources/Shaders/PostProcessing_Vert.glsl", "Resources/Shaders/PostProcessing_Frag.glsl"))
		std::cout << "Could not load post processing shader..." << std::endl;

	// Hide mouse
	this->window.setMouseCursorVisible(false);

	// Music
	if (this->gamePlayMusic.openFromFile("Resources/Sounds/Music/blackSwordsman.ogg") &&
		SettingsHandler::getMusicVolume() > 0)
	{
		this->gamePlayMusic.setVolume(SettingsHandler::getMusicVolume());
		this->gamePlayMusic.setLoop(true);
		this->gamePlayMusic.play();
	}
}

void PlayState::update(float deltaTime)
{
	// Update only if the window has focus
	if (this->window.hasFocus())
	{
		this->entityHandler.update(deltaTime);
		this->renderer.update(deltaTime);
		this->ui.update(deltaTime);
		this->gameStats.updateTimer(deltaTime);

		// Stop music if the player dies
		if (this->entityHandler.getPlayer().isHealthDepleted())
			this->gamePlayMusic.stop();

		// Player can and wants to exit
		if (this->entityHandler.getCollisionHandler().playerIsCloseToGoal() &&
			this->entityHandler.getPlayer().playerTriesToExit())
		{
			// Player won!
			this->gameStats.flagPlayerWon();

			// Switch state
			this->setState(State::GAME_OVER);
		}
		// Player has lost
		else if (this->entityHandler.playerHasLost())
		{
			// Set to slow end transition speed for a dramatic effect :p
			this->setStateTransitionSpeedScale(1, 0.4f);

			// Switch state
			this->setState(State::GAME_OVER);
		}
	}
}

void PlayState::render()
{
	// Render world
	sf::RenderTexture& renderTexture = renderer.render();

	// Render texture to screen
	this->postProcessingShader.setUniform("u_screenTexture", renderTexture.getTexture());
	this->postProcessingShader.setUniform("u_resolution", sf::Glsl::Vec2((float) SettingsHandler::getWidth(), (float) SettingsHandler::getHeight()));
	window.draw(this->screenRenderRect, &this->postProcessingShader);

	// Render entities
	this->entityHandler.render(this->window);

	// Render UI
	ui.render(window);
}
