#include "Player.h"

void Player::loadAnimations()
{
	// Load idle animation
	sf::IntRect swordIdleTextureRects[1]{ sf::IntRect(0, 0, SWORD_SPRITE_WIDTH, SWORD_SPRITE_HEIGHT) };
	this->swordIdleAnimation.init(
		1,
		swordIdleTextureRects,
		1.0f,
		false
	);

	// Load attack animation
	const unsigned int NUM_SWORD_ATTACK_TEXTURE_RECTS = 8;
	sf::IntRect swordAttackTextureRects[NUM_SWORD_ATTACK_TEXTURE_RECTS]{ };
	for (unsigned int i = 0; i < NUM_SWORD_ATTACK_TEXTURE_RECTS; ++i)
	{
		swordAttackTextureRects[i].left = (i % 2) * SWORD_SPRITE_WIDTH;
		swordAttackTextureRects[i].top = (i / 2) * SWORD_SPRITE_HEIGHT;
		swordAttackTextureRects[i].width = SWORD_SPRITE_WIDTH;
		swordAttackTextureRects[i].height = SWORD_SPRITE_HEIGHT;
	}
	this->swordAttackAnimation.init(
		NUM_SWORD_ATTACK_TEXTURE_RECTS,
		swordAttackTextureRects,
		0.1f,
		false
	);
}

void Player::updateSwordAnimationLogic(float deltaTime)
{
	this->isAttackingTimer -= deltaTime;
	this->isAttackingTimer = std::max(this->isAttackingTimer, 0.0f);

	// Update animation
	this->currentSwordAnimation->update(deltaTime);

	// Switch from attack animation to idle animation
	if (this->currentSwordAnimation == &this->swordAttackAnimation &&
		this->currentSwordAnimation->isDone())
	{
		this->currentSwordAnimation = &this->swordIdleAnimation;
	}

	// Switch to attack animation
	if (this->isAttackingTimer > 0.0f && !hasStartedAttackAnimation && 
		this->currentSwordAnimation != &this->swordAttackAnimation)
	{
		this->hasStartedAttackAnimation = true;

		this->currentSwordAnimation = &this->swordAttackAnimation;
		this->currentSwordAnimation->reset();

	}
}

void Player::updateSwordPosition()
{
	this->swordPosition = sf::Vector2f(0, 540 - 64 * SWORD_SPRITE_SCALE / 2 + 30);
	this->swordPosition.x += sin(walkTimer * 7.0f) * 50;
	this->swordPosition.y += sin(walkTimer * 7.0f * 2.0f) * 30;

	// Move to fit screen
	ResTranslator::transformSprite(
		this->swordSprite,
		this->swordPosition.x,
		this->swordPosition.y,
		192 * SWORD_SPRITE_SCALE,
		64 * SWORD_SPRITE_SCALE
	);

	// Set texture rect to current animation rect
	this->swordSprite.setTextureRect(this->currentSwordAnimation->getCurrentRect());
}

Player::Player(int x, int y)
	: x(x), y(y), direction(0.0f), walkTimer(0.0f), isAttackingTimer(0.0f),
	tryToExit(false), hasStartedAttackAnimation(false),
	currentSwordAnimation(&swordIdleAnimation)
{
	this->monitorMiddle = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);

	sf::Mouse::setPosition(this->monitorMiddle);

	// Load texture
	this->swordTextureSheet.loadFromFile("Resources/Textures/firstPersonSwordTexture.png");
	this->swordSprite.setTexture(this->swordTextureSheet);

	// Load animations
	this->loadAnimations();

	// Transform sprite
	this->updateSwordPosition();
}

void Player::handleInput(float deltaTime)
{
	// Try to exit
	this->tryToExit = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);

	// Attacking
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->isAttackingTimer <= 0.0f)
	{
		this->isAttackingTimer = 1.0f;
		this->hasStartedAttackAnimation = false;
	}

	// Grenade
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && this->grenadeCooldownTimer >= ABILITY_GRENADE_MAX_COOLDOWN_TIME)
		this->grenadeCooldownTimer = 0.0f;

	// Berserker
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && this->berserkerCooldownTimer >= ABILITY_BERSERKER_MAX_COOLDOWN_TIME)
		this->berserkerCooldownTimer = 0.0f;

	// Walking
	float forwardInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	float rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

	sf::Vector2f forwardDir(std::cos(-this->direction), std::sin(-this->direction));
	sf::Vector2f rightDir(-forwardDir.y, forwardDir.x);
	sf::Vector2f walkStep = forwardDir * forwardInput + rightDir * rightInput;
	SMath::vectorNormalize(walkStep);

	if (SMath::dot(walkStep, walkStep) > 0.0f)
		this->walkTimer += deltaTime;
	else
		this->walkTimer = 0.0f;

	// Move position
	this->x += walkStep.x * MOVEMENT_SPEED * deltaTime;
	this->y += walkStep.y * MOVEMENT_SPEED * deltaTime;


	// Keyboard to turn the player
	this->direction += (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) * 0.001f * SettingsHandler::getKeyboardLookSensitivity();
	
	// Mouse look
	this->direction += (this->monitorMiddle.x - sf::Mouse::getPosition().x) * 0.001f * SettingsHandler::getMouseSensitivity();

	// Move mouse to the middle of the monitor
	sf::Mouse::setPosition(this->monitorMiddle);
}

void Player::update(float deltaTime)
{
	// Update ability cooldowns
	this->grenadeCooldownTimer += deltaTime;
	this->berserkerCooldownTimer += deltaTime;
	this->grenadeCooldownTimer = SMath::clamp(this->grenadeCooldownTimer, 0.0f, ABILITY_GRENADE_MAX_COOLDOWN_TIME);
	this->berserkerCooldownTimer = SMath::clamp(this->berserkerCooldownTimer, 0.0f, ABILITY_BERSERKER_MAX_COOLDOWN_TIME);

	this->updateSwordAnimationLogic(deltaTime);
	this->updateSwordPosition();
}

void Player::render(sf::RenderWindow& window)
{
	window.draw(this->swordSprite);
}

const bool Player::playerTriesToExit() const
{
	return this->tryToExit;
}

const sf::Vector2f Player::getPlayerPosition() const
{
	return sf::Vector2f(x, y);
}

const sf::Glsl::Vec3 Player::getPlayerCamera() const
{
	return sf::Glsl::Vec3(x, y, direction);
}

float Player::getGrenadeCooldownPercent() const
{
	return this->grenadeCooldownTimer / ABILITY_GRENADE_MAX_COOLDOWN_TIME;
}

float Player::getBerserkerCooldownPercent() const
{
	return this->berserkerCooldownTimer / ABILITY_BERSERKER_MAX_COOLDOWN_TIME;
}
