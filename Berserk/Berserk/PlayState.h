#pragma once

#include "GameState.h"

class PlayState : public GameState
{
private:

public:
	PlayState(sf::RenderWindow& window);

	// Inherited via GameState
	virtual void handlePollEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};