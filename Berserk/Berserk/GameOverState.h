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
	sf::Text foundCollectiblesOutlineText;
	sf::Text playTimeOutlineText;
	sf::Text playTimeNumberOutlineText;
	sf::Text killedEnemiesOutlineText;
	sf::Text killedEnemiesNumberOutlineText;

	sf::Text foundCollectiblesText;
	sf::Text playTimeText;
	sf::Text playTimeNumberText;
	sf::Text killedEnemiesText;
	sf::Text killedEnemiesNumberText;

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