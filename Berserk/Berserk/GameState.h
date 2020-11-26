#pragma once

#include <SFML/Graphics.hpp>
#include "UITranslator.h"

enum class State
{
	MAIN_MENU,
	PLAY,
	GAME_OVER,

	NONE
};

class GameState
{
private:
	State switchToState;

protected:
	sf::RenderWindow& window;

	void setState(State newSwitchToState);

public:
	GameState(sf::RenderWindow& window);
	virtual ~GameState();

	virtual void handlePollEvent(const sf::Event& event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;

	State getSwitchToState() const;
};