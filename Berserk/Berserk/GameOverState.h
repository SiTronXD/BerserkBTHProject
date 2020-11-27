#pragma once

#include "GameState.h"
#include "Button.h"
#include "GameStatsHandler.h"
#include "ResTranslator.h"

class GameOverState : public GameState
{
private:
	Button restartButton;
	Button mainMenuButton;

	GameStatsHandler& gameStats;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

public:
	GameOverState(sf::RenderWindow& window, GameStatsHandler& gameStats);

	// Inherited via GameState
	virtual void handlePollEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};