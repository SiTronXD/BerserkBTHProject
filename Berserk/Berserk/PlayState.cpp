#include "PlayState.h"
#include <iostream>

PlayState::PlayState(sf::RenderWindow& window, GameStatsHandler& gameStats)
	: GameState(window), entityHandler(gameStats), mapHandler(entityHandler), renderer(mapHandler, entityHandler), 
	ui(entityHandler), gameStats(gameStats)
{
	// Reset and set stats
	this->gameStats.reset();
	this->gameStats.setMaxCollectibles(this->entityHandler.getNumCollectibles());

	// Set screen rectangle shape
	screenRenderRect.setSize(sf::Vector2f(SettingsHandler::getWidth(), SettingsHandler::getHeight()));
	screenRenderRect.setFillColor(sf::Color::Green);

	// Load post processing shader
	if (!postProcessingShader.loadFromFile("Resources/Shaders/PostProcessing_Vert.glsl", "Resources/Shaders/PostProcessing_Frag.glsl"))
		std::cout << "Could not load post processing shader..." << std::endl;

	// Hide mouse
	this->window.setMouseCursorVisible(false);
}

void PlayState::handlePollEvent(const sf::Event& event)
{
}

void PlayState::update(float deltaTime)
{
	this->entityHandler.update(deltaTime);
	this->renderer.update(deltaTime);
	this->ui.update(deltaTime);
	this->gameStats.updateTimer(deltaTime);

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
	else if(this->entityHandler.playerHasLost())
	{
		// Switch state
		this->setState(State::GAME_OVER);
	}
}

void PlayState::render()
{
	// Render world
	sf::RenderTexture& renderTexture = renderer.render();

	// Render texture to screen
	this->postProcessingShader.setUniform("u_screenTexture", renderTexture.getTexture());
	this->postProcessingShader.setUniform("u_resolution", sf::Glsl::Vec2(SettingsHandler::getWidth(), SettingsHandler::getHeight()));
	window.draw(this->screenRenderRect, &this->postProcessingShader);

	// Render entities
	this->entityHandler.render(this->window);

	// Render UI
	ui.render(window);
}
