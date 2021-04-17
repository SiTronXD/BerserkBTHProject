#pragma once

#include "../../Engine/GameState.h"
#include "../../Engine/Renderer.h"
#include "../GameStatsHandler.h"
#include "EntityHandler.h"
#include "MapHandler.h"
#include "UI.h"

class PlayState : public GameState
{
private:
	sf::RectangleShape screenRenderRect;
	sf::Shader postProcessingShader;

	sf::Music gamePlayMusic;

	EntityHandler entityHandler;
	MapHandler mapHandler;
	Renderer renderer;
	UI ui;
	GameStatsHandler& gameStats;

public:
	PlayState(sf::RenderWindow& window, GameStatsHandler& gameStats);

	// Inherited via GameState
	virtual void update(float deltaTime) override;
	virtual void render() override;
};