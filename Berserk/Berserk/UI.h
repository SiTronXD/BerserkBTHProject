#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "ResTranslator.h"
#include "Player.h"
#include "EntityHandler.h"

class UI
{
private:
	const float GRENADE_ICON_X = -740;
	const float GRENADE_ICON_Y = 470;
	const float BERSERKER_ICON_X = -880;
	const float BERSERKER_ICON_Y = 470;

	const float MAX_SHOW_MESSAGE_TIME = 3.0f;

	sf::Font font;
	sf::Text fpsText;
	sf::Text enterGoalText;
	sf::Text messageText;
	sf::Text healthText;

	sf::Texture grenadeIconTexture;
	sf::Texture berserkerIconTexture;
	sf::Texture healthIconTexture;
	sf::Texture redBoxTexture;
	sf::Texture blackBoxTexture;

	sf::Sprite grenadeIconSprite;
	sf::Sprite berserkerIconSprite;
	sf::Sprite healthIconSprite;
	sf::Sprite damageTakenSprite;
	sf::Sprite blackBarLeftSprite;
	sf::Sprite blackBarRightSprite;

	sf::Shader abilityIconShader;

	EntityHandler& entityHandler;
	Player& player;

	float grenadeIconScale;
	float berserkerIconScale;
	float updateDtTimer;
	float grenadeActiveTimer;
	float berserkerActiveTimer;
	float showMessageTimer;
	float damageTakenTimer;

	UI(const UI& other) = delete;
	UI operator=(const UI& other) = delete;

public:
	UI(EntityHandler& entityHandler);

	void update(float deltaTime);

	void render(sf::RenderWindow& window);
};