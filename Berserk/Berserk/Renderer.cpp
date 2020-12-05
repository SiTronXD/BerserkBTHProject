#include "Renderer.h"

Renderer::Renderer(MapHandler& mapHandler, EntityHandler& entityHandler) 
	: mapHandler(mapHandler), entityHandler(entityHandler), timer(0.0f), fogColor(0.0f, 0.0f, 0.0f)
{
	// Set screen rectangle shape
	shaderRenderRect.setSize(sf::Vector2f(SettingsHandler::getWidth(), SettingsHandler::getHeight()));
	shaderRenderRect.setFillColor(sf::Color::Green);

	// Create render texture
	if (!this->shaderRenderTexture.create(SettingsHandler::getWidth(), SettingsHandler::getHeight()))
		std::cout << "Could not create shaderRenderTexture..." << std::endl;

	// Check for shader availability
	if (!sf::Shader::isAvailable())
		std::cout << "Shaders are not available on this device..." << std::endl;

	// Load ray cast shader
	if (!rayCastShader.loadFromFile("Resources/Shaders/RayCast_Vert.glsl", "Resources/Shaders/RayCast_Frag.glsl"))
		std::cout << "Could not load ray cast shader..." << std::endl;

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
}

void Renderer::setFogColor(sf::Color color)
{
	this->fogColor.x = color.r / 255.0f;
	this->fogColor.y = color.g / 255.0f;
	this->fogColor.z = color.b / 255.0f;
}

sf::RenderTexture& Renderer::render()
{
	sf::Glsl::Vec2 resolution(SettingsHandler::getWidth(), SettingsHandler::getHeight());

	// Update shader uniforms
	this->rayCastShader.setUniform("u_timer", this->timer);
	this->rayCastShader.setUniform("u_cameraPosition", this->entityHandler.getPlayer().getPositionForRenderer());
	this->rayCastShader.setUniform("u_cameraRotation", this->entityHandler.getPlayer().getRotationForRenderer());
	this->rayCastShader.setUniform("u_mapTexture", this->mapHandler.getMapTexture());
	this->rayCastShader.setUniform("u_wallTexture", this->wallTexture);
	this->rayCastShader.setUniform("u_floorTexture", this->floorTexture);
	this->rayCastShader.setUniform("u_goalTexture", this->goalTexture);
	this->rayCastShader.setUniform("u_resolution", resolution);
	this->rayCastShader.setUniform("u_fogColor", this->fogColor);

	// Fill array with entity positions and texture rects
	sf::Glsl::Vec3 entityPositionsArray[MAX_ENTITIES] { };
	sf::Glsl::Vec4 entityTexRectArray[MAX_ENTITIES]{ };
	sf::Glsl::Vec2 entityWorldScale[MAX_ENTITIES]{ };
	int arraySize = 0;
	this->entityHandler.fillArraysWithEntityArrays(entityPositionsArray, entityTexRectArray, 
		entityWorldScale, arraySize);

	// Update entity uniforms
	this->rayCastShader.setUniform("u_entityTexture", this->entitiesTexture);
	this->rayCastShader.setUniformArray("u_entityPositions", entityPositionsArray, MAX_ENTITIES);
	this->rayCastShader.setUniformArray("u_entityTexRects", entityTexRectArray, MAX_ENTITIES);
	this->rayCastShader.setUniformArray("u_entityWorldScales", entityWorldScale, MAX_ENTITIES);
	this->rayCastShader.setUniform("u_numEntities", arraySize);

	// Render
	this->shaderRenderTexture.draw(this->shaderRenderRect, &this->rayCastShader);
	this->shaderRenderTexture.display();

	return this->shaderRenderTexture;
}
