#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Button.h"

class MainMenuState : public GameState
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	Button playButton;
	Button exitButton;

public:
	MainMenuState(sf::RenderWindow& window);
	~MainMenuState();

	// Inherited via GameState
	virtual void handlePollEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};