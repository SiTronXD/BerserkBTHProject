#include "GameState.h"

GameState::GameState(sf::RenderWindow& window)
	: window(window), switchToState(State::NONE),
	transitionStartSpeed(1.0f), transitionEndSpeed(1.0f)
{ }

GameState::~GameState()
{ }

void GameState::setState(State newSwitchToState)
{
	this->switchToState = newSwitchToState;
}

void GameState::setStateTransitionSpeedScale(int dir, float speedScale)
{
	if (dir < 0)
		this->transitionStartSpeed = speedScale;
	else
		this->transitionEndSpeed = speedScale;
}

State GameState::getSwitchToState() const
{
	return this->switchToState;
}

float GameState::getSpecificStateTransitionSpeedScale(int transitionDir) const
{
	if (transitionDir < 0)
		return this->transitionStartSpeed;

	return this->transitionEndSpeed;
}
