#include "Renderer.h"
#include <iostream>

Renderer::Renderer(MapHandler& mapHandler, EntityHandler& entityHandler) 
	: mapHandler(mapHandler), entityHandler(entityHandler), timer(0.0f), fogColor(0.0f, 0.0f, 0.0f)
{
	// Set screen rectangle shapes
	rayCastRenderRect.setSize(sf::Vector2f((float)SettingsHandler::getWidth(), 1.0f));
	rayCastRenderRect.setFillColor(sf::Color::Green);
	shaderRenderRect.setSize(sf::Vector2f((float) SettingsHandler::getWidth(), (float) SettingsHandler::getHeight()));
	shaderRenderRect.setFillColor(sf::Color::Green);

	// Create render textures
	if (!this->rayCastRenderTexture.create(SettingsHandler::getWidth(), 1))
		std::cout << "Could not create rayCastRenderTexture..." << std::endl;
	if (!this->shaderRenderTexture.create(SettingsHandler::getWidth(), SettingsHandler::getHeight()))
		std::cout << "Could not create shaderRenderTexture..." << std::endl;

	// Check for shader availability
	if (!sf::Shader::isAvailable())
		std::cout << "Shaders are not available on this device..." << std::endl;

	// Load ray cast shaders
	if (!rayCastShader.loadFromFile("Resources/Shaders/RayCastMain_Vert.glsl", "Resources/Shaders/RayCastMain_Frag.glsl"))
		std::cout << "Could not load ray cast shader..." << std::endl;
	if (!rayCastCompositingShader.loadFromFile("Resources/Shaders/RayCastCompositing_Vert.glsl", "Resources/Shaders/RayCastCompositing_Frag.glsl"))
		std::cout << "Could not load ray cast compositing shader..." << std::endl;

	// Disable alpha blending
	this->renderState.blendMode = sf::BlendNone;
	this->renderState.shader = &this->rayCastShader;

	// Load textures
	this->wallTexture.loadFromFile("Resources/Textures/eclipseWallTexture.png");
	this->floorTexture.loadFromFile("Resources/Textures/floorTexture2.png");
	this->goalTexture.loadFromFile("Resources/Textures/goal.png");
	this->entitiesTexture.loadFromFile("Resources/Textures/RenderEntityTextureSheet.png");
}

void Renderer::update(float deltaTime)
{
	this->timer += deltaTime;

	// Update current fog color
	this->setFogColor(this->entityHandler.getCurrentFogColor());
}

void Renderer::setFogColor(sf::Color color)
{
	this->fogColor.x = color.r / 255.0f;
	this->fogColor.y = color.g / 255.0f;
	this->fogColor.z = color.b / 255.0f;
}

sf::RenderTexture& Renderer::render()
{
	sf::Glsl::Vec2 resolution((float) SettingsHandler::getWidth(), (float) SettingsHandler::getHeight());

	// Update ray cast shader uniforms
	this->rayCastShader.setUniform("u_cameraPosition", this->entityHandler.getPlayer().getPositionForRenderer());
	this->rayCastShader.setUniform("u_cameraRotation", this->entityHandler.getPlayer().getRotationForRenderer());
	this->rayCastShader.setUniform("u_mapTexture", this->mapHandler.getMapTexture());
	this->rayCastShader.setUniform("u_resolution", resolution);

	// Render ray cast
	this->rayCastRenderTexture.draw(this->rayCastRenderRect, this->renderState);
	this->rayCastRenderTexture.display();

	// Update ray cast compositing shader
	this->rayCastCompositingShader.setUniform("u_rayCastTexture", this->rayCastRenderTexture.getTexture());
	this->rayCastCompositingShader.setUniform("u_cameraPosition", this->entityHandler.getPlayer().getPositionForRenderer());
	this->rayCastCompositingShader.setUniform("u_cameraRotation", this->entityHandler.getPlayer().getRotationForRenderer());
	this->rayCastCompositingShader.setUniform("u_timer", this->timer);
	this->rayCastCompositingShader.setUniform("u_mapTexture", this->mapHandler.getMapTexture());
	this->rayCastCompositingShader.setUniform("u_wallTexture", this->wallTexture);
	this->rayCastCompositingShader.setUniform("u_floorTexture", this->floorTexture);
	this->rayCastCompositingShader.setUniform("u_goalTexture", this->goalTexture);
	this->rayCastCompositingShader.setUniform("u_fogColor", this->fogColor);
	this->rayCastCompositingShader.setUniform("u_resolution", resolution);

	// Fill array with entity positions and texture rects
	sf::Glsl::Vec3 entityPositionsArray[MAX_ENTITIES] { };
	sf::Glsl::Vec4 entityTexRectArray[MAX_ENTITIES]{ };
	sf::Glsl::Vec2 entityWorldScale[MAX_ENTITIES]{ };
	int arraySize = 0;
	this->entityHandler.fillArraysWithEntityArrays(entityPositionsArray, entityTexRectArray, 
		entityWorldScale, arraySize);

	// Update entity uniforms
	this->rayCastCompositingShader.setUniform("u_entityTexture", this->entitiesTexture);
	this->rayCastCompositingShader.setUniformArray("u_entityPositions", entityPositionsArray, MAX_ENTITIES);
	this->rayCastCompositingShader.setUniformArray("u_entityTexRects", entityTexRectArray, MAX_ENTITIES);
	this->rayCastCompositingShader.setUniformArray("u_entityWorldScales", entityWorldScale, MAX_ENTITIES);
	this->rayCastCompositingShader.setUniform("u_numEntities", arraySize);

	// Render
	this->shaderRenderTexture.draw(this->shaderRenderRect, &this->rayCastCompositingShader);
	this->shaderRenderTexture.display();

	return this->shaderRenderTexture;
}
