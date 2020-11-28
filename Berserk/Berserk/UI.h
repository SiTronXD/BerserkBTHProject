#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "ResTranslator.h"
#include "CollisionHandler.h"
#include "Player.h"

class UI
{
private:
	const float GRENADE_ICON_X = -740;
	const float GRENADE_ICON_Y = 470;
	const float BERSERKER_ICON_X = -880;
	const float BERSERKER_ICON_Y = 470;

	sf::Font font;
	sf::Text fpsText;
	sf::Text enterGoalText;

	sf::Texture grenadeIconTexture;
	sf::Texture berserkerIconTexture;
	sf::Sprite grenadeIconSprite;
	sf::Sprite berserkerIconSprite;

	sf::Shader abilityIconShader;

	CollisionHandler& collisionHandler;
	Player& player;

	float grenadeIconScale;
	float berserkerIconScale;
	float updateDtTimer;
	float grenadeActiveTimer;
	float berserkerActiveTimer;

public:
	UI(CollisionHandler& collisionHandler, Player& player);

	void update(float deltaTime);

	void render(sf::RenderWindow& window);
};