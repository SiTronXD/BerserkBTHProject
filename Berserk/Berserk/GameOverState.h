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

	sf::Font font;
	OutlineText foundCollectiblesText;
	OutlineText playTimeText;
	OutlineText playTimeNumberText;
	OutlineText killedEnemiesText;
	OutlineText killedEnemiesNumberText;

	sf::Music gameOverMusic;
	
	GameStatsHandler& gameStats;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

public:
	GameOverState(sf::RenderWindow& window, GameStatsHandler& gameStats);

	// Inherited via GameState
	virtual void update(float deltaTime) override;
	virtual void render() override;
};