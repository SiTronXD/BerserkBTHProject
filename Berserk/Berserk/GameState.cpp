#include "GameState.h"

GameState::GameState(sf::RenderWindow& window)
	: window(window), switchToState(State::NONE)
{ }

GameState::~GameState()
{ }

void GameState::setState(State newSwitchToState)
{
	this->switchToState = newSwitchToState;
}

State GameState::getSwitchToState() const
{
	return this->switchToState;
}
