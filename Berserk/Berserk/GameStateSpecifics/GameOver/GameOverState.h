#pragma once

#include "../../Engine/GameState.h"
#include "../../Engine/Button.h"
#include "../../Engine/ResTranslator.h"
#include "../GameStatsHandler.h"

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