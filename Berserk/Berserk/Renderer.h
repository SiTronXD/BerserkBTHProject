#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MapHandler.h"
#include "SettingsHandler.h"
#include "EntityHandler.h"

class Renderer
{
private:
	static const int MAX_ENTITIES = 64;

	sf::RectangleShape shaderRenderRect;
	sf::RenderTexture shaderRenderTexture;
	sf::RenderStates renderState;

	sf::Texture wallTexture;
	sf::Texture floorTexture;
	sf::Texture goalTexture;
	sf::Texture entitiesTexture;

	sf::Shader rayCastShader;

	MapHandler& mapHandler;
	EntityHandler& entityHandler;

	float timer;

public:
	Renderer(MapHandler& mapHandler, EntityHandler& entityHandler);

	void update(float deltaTime);

	sf::RenderTexture& render();
};