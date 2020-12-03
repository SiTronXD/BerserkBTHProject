#include "UI.h"

UI::UI(CollisionHandler& collisionHandler, Player& player)
	: updateDtTimer(1.0f), collisionHandler(collisionHandler), player(player)
{
	this->font.loadFromFile("Resources/Fonts/Pixellari.ttf");

	// Frames per second text
	this->fpsText.setFont(this->font);
	this->fpsText.setFillColor(sf::Color::Red);
	this->fpsText.setString("FPS: ");
	ResTranslator::transformText(this->fpsText, -850, -500, 40);

	// Press [F] to exit text
	this->enterGoalText.setFont(this->font);
	this->enterGoalText.setFillColor(sf::Color::Red);
	this->enterGoalText.setString("PRESS [F] TO EXIT");
	ResTranslator::transformText(this->enterGoalText, 0, 100, 40);

	// Message text
	this->messageText.setFont(this->font);
	this->messageText.setFillColor(sf::Color::Green);
	this->messageText.setString("<Message>");

	// Grenade icon
	this->grenadeIconTexture.loadFromFile("Resources/Textures/GrenadeIcon.png");
	this->grenadeIconSprite.setTexture(this->grenadeIconTexture);
	ResTranslator::transformSprite(this->grenadeIconSprite, GRENADE_ICON_X, GRENADE_ICON_Y, 100, 100);
	this->grenadeIconScale = this->grenadeIconSprite.getScale().x;

	// Berserker icon
	this->berserkerIconTexture.loadFromFile("Resources/Textures/BeastOfDarknessIcon.png");
	this->berserkerIconSprite.setTexture(this->berserkerIconTexture);
	ResTranslator::transformSprite(this->berserkerIconSprite, BERSERKER_ICON_X, BERSERKER_ICON_Y, 100, 100);
	this->berserkerIconScale = this->grenadeIconSprite.getScale().x;

	// Load shader
	if (!abilityIconShader.loadFromFile("Resources/Shaders/AbilityIcon_Vert.glsl", "Resources/Shaders/AbilityIcon_Frag.glsl"))
		std::cout << "Could not load ability icon shader..." << std::endl;
}

void UI::update(float deltaTime)
{
	this->updateDtTimer += deltaTime;

	// Update frames per second text
	if (this->updateDtTimer >= 0.25f)
	{
		std::string fpsText = "FPS: " + std::to_string((int)(1.0f / deltaTime));
		this->fpsText.setString(fpsText);

		this->updateDtTimer = 0.0f;
	}

	// Grenade is ready
	if (this->player.getGrenadeCooldownPercent() >= 1.0f)
		this->grenadeActiveTimer += deltaTime;
	else
		this->grenadeActiveTimer = 0.0f;

	// Berserker is ready
	if (this->player.getBerserkerCooldownPercent() >= 1.0f)
		this->berserkerActiveTimer += deltaTime;
	else
		this->berserkerActiveTimer = 0.0f;

	// Update grenade scale
	float grenadeScaleFactor = (1.0f + 0.2f * abs(sin(this->grenadeActiveTimer * 4.0f)));
	ResTranslator::transformSprite(this->grenadeIconSprite, GRENADE_ICON_X, GRENADE_ICON_Y, 100 * grenadeScaleFactor, 100 * grenadeScaleFactor);

	// Update berserker scale
	float berserkerScaleFactor = (1.0f + 0.2f * abs(sin(this->berserkerActiveTimer * 4.0f)));
	ResTranslator::transformSprite(this->berserkerIconSprite, BERSERKER_ICON_X, BERSERKER_ICON_Y, 100 * berserkerScaleFactor, 100 * berserkerScaleFactor);

	// Message timer
	this->showMessageTimer -= deltaTime;
	this->showMessageTimer = std::max(this->showMessageTimer, 0.0f);

	// Set message
	std::string collisionMsgText = this->collisionHandler.getUIMessage();
	if (collisionMsgText != "")
	{
		this->showMessageTimer = MAX_SHOW_MESSAGE_TIME;
		this->messageText.setString(collisionMsgText);
		ResTranslator::transformText(this->messageText, 0, -300, 60);
	}
}

void UI::render(sf::RenderWindow& window)
{
	window.draw(this->fpsText);

	// Grenade icon
	this->abilityIconShader.setUniform("u_percentage", this->player.getGrenadeCooldownPercent());
	window.draw(this->grenadeIconSprite, &this->abilityIconShader);

	// Berserker icon
	this->abilityIconShader.setUniform("u_percentage", this->player.getBerserkerCooldownPercent());
	window.draw(this->berserkerIconSprite, &this->abilityIconShader);

	// Message
	if(this->showMessageTimer > 0.0f)
		window.draw(this->messageText);

	// Show exit text if necessary
	if (collisionHandler.playerIsCloseToGoal())
		window.draw(this->enterGoalText);
}
