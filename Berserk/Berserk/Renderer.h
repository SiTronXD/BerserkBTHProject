#pragma once

#include <SFML/Graphics.hpp>
#include "MapHandler.h"
#include "SettingsHandler.h"
#include "EntityHandler.h"

class Renderer
{
private:
	static const int MAX_ENTITIES = 128;

	sf::RectangleShape shaderRenderRect;
	sf::RenderTexture shaderRenderTexture;
	sf::RenderStates renderState;

	sf::Texture wallTexture;
	sf::Texture floorTexture;
	sf::Texture goalTexture;
	sf::Texture entitiesTexture;

	sf::Shader rayCastShader;

	sf::Glsl::Vec3 fogColor;

	MapHandler& mapHandler;
	EntityHandler& entityHandler;

	float timer;

	Renderer(const Renderer& other) = delete;
	Renderer operator=(const Renderer& other) = delete;

public:
	Renderer(MapHandler& mapHandler, EntityHandler& entityHandler);

	void update(float deltaTime);

	void setFogColor(sf::Color color);

	sf::RenderTexture& render();
};