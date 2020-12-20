#include "UI.h"

UI::UI(EntityHandler& entityHandler)
	: entityHandler(entityHandler), player(entityHandler.getPlayer()),
	updateDtTimer(1.0f), berserkerActiveTimer(0.0f), grenadeActiveTimer(0.0f),
	damageTakenTimer(0.0f), showMessageTimer(0.0f), frameCounter(0)
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

	// Health text
	this->healthText.setFont(this->font);
	this->healthText.setFillColor(sf::Color::Red);
	this->healthText.setString("100");

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

	// Health icon
	this->healthIconTexture.loadFromFile("Resources/Textures/HealthTexture.png");
	this->healthIconSprite.setTexture(this->healthIconTexture);
	ResTranslator::transformSprite(this->healthIconSprite, 765, 475, 50, 50);

	// Damage taken
	this->redBoxTexture.loadFromFile("Resources/Textures/RedBox.png");
	this->damageTakenSprite.setTexture(this->redBoxTexture);
	ResTranslator::transformSprite(this->damageTakenSprite, 0, 0, (float) ResTranslator::getVirtualWidth(), 1080);

	// Black bars
	int virtualUIWidth = ResTranslator::getVirtualWidth();
	int blackBarWidth = (int) std::ceil((virtualUIWidth - 1920) * 0.5f);
	int blackBarHorizontalPos = (int) (blackBarWidth * 0.5f + 1920 * 0.5f);
	this->blackBoxTexture.loadFromFile("Resources/Textures/BlackBox.png");
	this->blackBarLeftSprite.setTexture(this->blackBoxTexture);
	this->blackBarRightSprite.setTexture(this->blackBoxTexture);
	ResTranslator::transformSprite(this->blackBarLeftSprite, (float) -blackBarHorizontalPos, 0, (float) blackBarWidth, 1080);
	ResTranslator::transformSprite(this->blackBarRightSprite, (float) blackBarHorizontalPos, 0, (float) blackBarWidth, 1080);

	// Load shader
	if (!abilityIconShader.loadFromFile("Resources/Shaders/AbilityIcon_Vert.glsl", "Resources/Shaders/AbilityIcon_Frag.glsl"))
		std::cout << "Could not load ability icon shader..." << std::endl;
}

void UI::update(float deltaTime)
{
	this->updateDtTimer += deltaTime;

	// Update frames per second text
	if (this->updateDtTimer >= 1.0f)
	{
		std::string fpsText = "FPS: " + std::to_string(this->frameCounter);
		this->fpsText.setString(fpsText);

		this->updateDtTimer = 0.0f;
		this->frameCounter = 0;
	}
	else
		this->frameCounter++;

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
	std::string msgText = this->entityHandler.getUIMessage();
	if (msgText != "")
	{
		this->showMessageTimer = MAX_SHOW_MESSAGE_TIME;
		this->messageText.setString(msgText);
		ResTranslator::transformText(this->messageText, 0, -300, 60);
	}

	// Check if damage has been taken
	if (this->entityHandler.isPlayerTakingDamage())
	{
		this->damageTakenTimer = 0.3f;
	}
	else
	{
		this->damageTakenTimer -= deltaTime;
		this->damageTakenTimer = std::max(this->damageTakenTimer, 0.0f);
	}

	// Update damage taken alpha color
	sf::Color newSpriteColor = sf::Color(255, 255, 255, (sf::Uint8) (255 * this->damageTakenTimer));
	this->damageTakenSprite.setColor(newSpriteColor);

	// Update health text
	this->healthText.setString(std::to_string(this->player.getCurrentHealth()));
	ResTranslator::transformText(this->healthText, 850, 450, 40);

	// Update black bar color
	float berserkerAlpha = this->entityHandler.getPlayer().getBerserkerBlackBarAlpha();
	sf::Color blackBarColor = sf::Color(255, 255, 255, (sf::Uint8) (255 * berserkerAlpha));
	this->blackBarLeftSprite.setColor(blackBarColor);
	this->blackBarRightSprite.setColor(blackBarColor);
}

void UI::render(sf::RenderWindow& window)
{
	// Black bars
	window.draw(this->blackBarLeftSprite);
	window.draw(this->blackBarRightSprite);

	// Frames per second
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

	// Health
	window.draw(this->healthText);
	window.draw(this->healthIconSprite);

	// Show exit text if necessary
	if (entityHandler.playerIsCloseToGoal())
		window.draw(this->enterGoalText);

	// Damage taken
	if (this->damageTakenTimer > 0.0f)
		window.draw(this->damageTakenSprite);
}
