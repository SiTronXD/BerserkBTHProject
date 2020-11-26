#pragma once

#include "GameState.h"
#include "UI.h"
#include "EntityHandler.h"
#include "Renderer.h"

class PlayState : public GameState
{
private:
	sf::RectangleShape screenRenderRect;
	sf::Shader postProcessingShader;

	UI ui;
	EntityHandler entityHandler;
	Renderer renderer;

public:
	PlayState(sf::RenderWindow& window);

	// Inherited via GameState
	virtual void handlePollEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};