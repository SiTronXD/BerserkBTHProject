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
	sf::RenderWindow window;
	sf::Clock deltaTimeClock;

	GameStatsHandler gameStats;

	GameState* currentState;

	void handlePollEvent(const sf::Event& event);
	void update();
	void render();

	void setState(State newState);
	void setWindowIcon();

public:
	Game();
	~Game();

	void run();
};