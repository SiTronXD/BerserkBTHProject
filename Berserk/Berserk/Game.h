#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "MainMenuState.h"
#include "PlayState.h"

class Game
{
private:
	sf::RenderWindow window;

	GameState* currentState;

	void handlePollEvent(const sf::Event& event);
	void update();
	void render();

	void setState(State newState);

public:
	Game();
	~Game();

	void run();
};