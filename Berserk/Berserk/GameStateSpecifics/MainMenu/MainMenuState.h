#pragma once

#include <SFML/Graphics.hpp>
#include "../../Engine/GameState.h"
#include "../../Engine/Button.h"

class MainMenuState : public GameState
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Music menuMusic;

	Button playButton;
	Button exitButton;

public:
	MainMenuState(sf::RenderWindow& window);
	~MainMenuState();

	// Inherited via GameState
	virtual void update(float deltaTime) override;
	virtual void render() override;
};