#include "PlayState.h"
#include <iostream>

PlayState::PlayState(sf::RenderWindow& window)
	: GameState(window), mapHandler(entityHandler), renderer(mapHandler, entityHandler)
{ 
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
}

void PlayState::render()
{
	// Render world
	sf::RenderTexture& renderTexture = renderer.render();

	// Render texture to screen
	this->postProcessingShader.setUniform("u_screenTexture", renderTexture.getTexture());
	this->postProcessingShader.setUniform("u_resolution", sf::Glsl::Vec2(SettingsHandler::getWidth(), SettingsHandler::getHeight()));
	window.draw(this->screenRenderRect, &this->postProcessingShader);

	// Render UI
	ui.render(window);
}
