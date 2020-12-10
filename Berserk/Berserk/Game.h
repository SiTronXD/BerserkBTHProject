#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "MainMenuState.h"
#include "PlayState.h"
#include "GameOverState.h"
#include "GameStatsHandler.h"

class Game
{
private:
	const float TRANSITION_SPEED_SCALE = 2.0f;

	sf::RenderWindow window;
	sf::Clock deltaTimeClock;

	sf::Texture blackBoxTexture;
	sf::Sprite transitionSprite;

	GameStatsHandler gameStats;

	GameState* currentState;
	State stateToSwitchTo;

	float transitionAlpha;
	int transitionDirection;

	void handlePollEvent(const sf::Event& event);
	void update();
	void render();

	void setState(State newState);
	void applyNewState();
	void applySettings();
	void setWindowIcon();
	void setupTransition();

public:
	Game();
	~Game();

	void run();
};