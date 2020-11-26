#include "Renderer.h"

Renderer::Renderer(EntityHandler& entityHandler) 
	: entityHandler(entityHandler), timer(0.0f)
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
	this->map.loadFromFile("Resources/Maps/TestLevel.png");
	this->wallTexture.loadFromFile("Resources/Textures/eclipseWallTexture.png");
	this->floorTexture.loadFromFile("Resources/Textures/floorTexture2.png");
}

void Renderer::update(float deltaTime)
{
	this->timer += deltaTime;
}

sf::RenderTexture& Renderer::render()
{
	sf::Glsl::Vec2 resolution(SettingsHandler::getWidth(), SettingsHandler::getHeight());

	// Render rectangle using the shader
	this->rayCastShader.setUniform("u_timer", this->timer);
	this->rayCastShader.setUniform("u_camera", this->entityHandler.getPlayer().getPlayerCamera());
	this->rayCastShader.setUniform("u_mapTexture", this->map);
	this->rayCastShader.setUniform("u_wallTexture", this->wallTexture);
	this->rayCastShader.setUniform("u_floorTexture", this->floorTexture);
	this->rayCastShader.setUniform("u_resolution", resolution);
	this->shaderRenderTexture.draw(this->shaderRenderRect, &this->rayCastShader);
	this->shaderRenderTexture.display();

	return this->shaderRenderTexture;
}
