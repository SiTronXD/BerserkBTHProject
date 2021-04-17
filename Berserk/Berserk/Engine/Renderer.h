#pragma once

#include <SFML/Graphics.hpp>
#include "../GameStateSpecifics/PlayState/MapHandler.h"
#include "../GameStateSpecifics/PlayState/EntityHandler.h"
#include "SettingsHandler.h"

class Renderer
{
private:
	static const int MAX_ENTITIES = 128;

	sf::RectangleShape rayCastRenderRect;
	sf::RectangleShape shaderRenderRect;
	sf::RenderTexture shaderRenderTexture;
	sf::RenderTexture rayCastRenderTexture;
	sf::RenderStates renderState;

	sf::Texture wallTexture;
	sf::Texture floorTexture;
	sf::Texture goalTexture;
	sf::Texture entitiesTexture;

	sf::Shader rayCastShader;
	sf::Shader rayCastCompositingShader;

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