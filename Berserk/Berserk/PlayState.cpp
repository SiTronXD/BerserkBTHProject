#include "PlayState.h"
#include <iostream>

PlayState::PlayState(sf::RenderWindow& window)
	: GameState(window), renderer(entityHandler)
{ 
	// Set screen rectangle shape
	screenRenderRect.setSize(sf::Vector2f(SettingsHandler::getWidth(), SettingsHandler::getHeight()));
	screenRenderRect.setFillColor(sf::Color::Green);

	// Load post processing shader
	if (!postProcessingShader.loadFromFile("Resources/Shaders/PostProcessing_Vert.glsl", "Resources/Shaders/PostProcessing_Frag.glsl"))
		std::cout << "Could not load post processing shader..." << std::endl;
}

void PlayState::handlePollEvent(const sf::Event& event)
{
}

void PlayState::update(float deltaTime)
{
	this->entityHandler.update(deltaTime);
}

void PlayState::render()
{
	sf::Glsl::Vec2 resolution(SettingsHandler::getWidth(), SettingsHandler::getHeight());

	sf::RenderTexture& renderTexture = renderer.render();

	// Render rectangle to screen
	this->postProcessingShader.setUniform("u_screenTexture", renderTexture.getTexture());
	this->postProcessingShader.setUniform("u_resolution", resolution);
	window.draw(this->screenRenderRect, &this->postProcessingShader);
}
