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
	sf::Shader rayCastShader;

	EntityHandler& entityHandler;

public:
	Renderer(EntityHandler& entityHandler);

	sf::RenderTexture& render();
};