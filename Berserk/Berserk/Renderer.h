#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "SettingsHandler.h"
#include "EntityHandler.h"

class Renderer
{
private:
	sf::RectangleShape shaderRenderRect;
	sf::RenderTexture shaderRenderTexture;
	sf::RenderStates renderState;

	sf::Texture map;
	sf::Texture wallTexture;
	sf::Texture floorTexture;

	sf::Shader rayCastShader;

	EntityHandler& entityHandler;

	float timer;

public:
	Renderer(EntityHandler& entityHandler);

	void update(float deltaTime);

	sf::RenderTexture& render();
};