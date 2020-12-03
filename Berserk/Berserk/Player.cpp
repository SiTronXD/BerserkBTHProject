#include "Player.h"
#include "EntityHandler.h"

void Player::loadAnimations()
{
	// Load idle animation
	sf::IntRect swordIdleTextureRects[1]{ sf::IntRect(0, 0, FIRST_PERSON_SPRITE_WIDTH, FIRST_PERSON_SPRITE_HEIGHT) };
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
		swordAttackTextureRects[i].left = (i % 2) * FIRST_PERSON_SPRITE_WIDTH;
		swordAttackTextureRects[i].top = (i / 2) * FIRST_PERSON_SPRITE_HEIGHT;
		swordAttackTextureRects[i].width = FIRST_PERSON_SPRITE_WIDTH;
		swordAttackTextureRects[i].height = FIRST_PERSON_SPRITE_HEIGHT;
	}
	this->swordAttackAnimation.init(
		NUM_SWORD_ATTACK_TEXTURE_RECTS,
		swordAttackTextureRects,
		0.1f,
		false
	);

	// Load grenade throw animation
	const unsigned int NUM_GRENADE_THROW_TEXTURE_RECTS = 5;
	sf::IntRect grenadeThrowTextureRects[NUM_GRENADE_THROW_TEXTURE_RECTS]{ };
	for (unsigned int i = 0; i < NUM_GRENADE_THROW_TEXTURE_RECTS; ++i)
	{
		grenadeThrowTextureRects[i].left = 0;
		grenadeThrowTextureRects[i].top = i * FIRST_PERSON_SPRITE_HEIGHT;
		grenadeThrowTextureRects[i].width = FIRST_PERSON_SPRITE_WIDTH;
		grenadeThrowTextureRects[i].height = FIRST_PERSON_SPRITE_HEIGHT;
	}
	this->grenadeThrowAnimation.init(
		NUM_GRENADE_THROW_TEXTURE_RECTS,
		grenadeThrowTextureRects,
		0.11f,
		false
	);

	// Load start berserk animation
	const unsigned int NUM_START_BERSERK_TEXTURE_RECTS = 5;
	sf::IntRect startBerserkTextureRects[NUM_START_BERSERK_TEXTURE_RECTS];
	for (unsigned int i = 0; i < NUM_START_BERSERK_TEXTURE_RECTS; ++i)
	{
		startBerserkTextureRects[i].left = 0;
		startBerserkTextureRects[i].top = i * BERSERK_SPRITE_HEIGHT;
		startBerserkTextureRects[i].width = BERSERK_SPRITE_WIDTH;
		startBerserkTextureRects[i].height = BERSERK_SPRITE_HEIGHT;
	}
	this->startBerserkAnimation.init(
		NUM_START_BERSERK_TEXTURE_RECTS,
		startBerserkTextureRects,
		0.12f,
		false
	);

	// Load end berserk animation
	const unsigned int NUM_END_BERSERK_TEXTURE_RECTS = NUM_START_BERSERK_TEXTURE_RECTS;
	sf::IntRect endBerserkTextureRects[NUM_END_BERSERK_TEXTURE_RECTS];
	for (unsigned int i = 0; i < NUM_END_BERSERK_TEXTURE_RECTS; ++i)
	{
		endBerserkTextureRects[i].left = 0;
		endBerserkTextureRects[i].top = (NUM_END_BERSERK_TEXTURE_RECTS - 1 - i) * BERSERK_SPRITE_HEIGHT;
		endBerserkTextureRects[i].width = BERSERK_SPRITE_WIDTH;
		endBerserkTextureRects[i].height = BERSERK_SPRITE_HEIGHT;
	}
	this->endBerserkAnimation.init(
		NUM_END_BERSERK_TEXTURE_RECTS,
		endBerserkTextureRects,
		0.12f,
		false
	);

	this->berserkSprite.setTexture(startBerserkTextureSheet);
}

void Player::spawnGrenade()
{
	// Remove these, just to be sure
	delete this->grenade;
	delete this->grenadeExplosion;
	this->grenade = nullptr;
	this->grenadeExplosion = nullptr;

	float playerAngle = this->getOrientation().z;
	sf::Vector2f direction = sf::Vector2f(cos(playerAngle), -sin(playerAngle));
	sf::Vector2f spawnPos = this->getPlayerPosition() + direction;
	this->grenade = new Grenade(spawnPos, direction);
}

void Player::updateAnimationLogic(float deltaTime)
{
	this->isAttackingTimer -= deltaTime;
	this->isAttackingTimer = std::max(this->isAttackingTimer, 0.0f);

	// Update animations
	this->currentFpsAnimation->update(deltaTime);

	if(this->currentBerserkAnimation)
		this->currentBerserkAnimation->update(deltaTime);

	// Switch from attack animation to idle animation
	if (this->currentFpsAnimation == &this->swordAttackAnimation &&
		this->currentFpsAnimation->isDone())
	{
		this->currentFpsAnimation = &this->swordIdleAnimation;
	}

	// Switch to attack animation
	if (this->isAttackingTimer > 0.0f && !hasStartedAttackAnimation && 
		this->currentFpsAnimation != &this->swordAttackAnimation)
	{
		this->hasStartedAttackAnimation = true;

		this->currentFpsAnimation = &this->swordAttackAnimation;
		this->currentFpsAnimation->reset();

		// Set texture
		this->handsSprite.setTexture(this->swordTextureSheet);
	}

	// Switch to grenade throw animation
	if (this->startThrowAnimation && this->currentFpsAnimation != &this->grenadeThrowAnimation)
	{
		this->startThrowAnimation = false;
		this->hasSpawnedGrenade = false;

		// Set and reset animation
		this->currentFpsAnimation = &this->grenadeThrowAnimation;
		this->currentFpsAnimation->reset();

		// Set texture
		this->handsSprite.setTexture(this->grenadeThrowTextureSheet);
	}

	// Spawn grenade if the animation has passed the correct index
	if (this->currentFpsAnimation == &this->grenadeThrowAnimation && 
		this->currentFpsAnimation->getCurrentRectIndex() >= 2 && !hasSpawnedGrenade)
	{
		hasSpawnedGrenade = true;

		// Spawn grenade
		this->spawnGrenade();
	}

	// Switch away from grenade throw animation
	if (this->currentFpsAnimation == &this->grenadeThrowAnimation && this->currentFpsAnimation->isDone())
	{
		this->currentFpsAnimation = &this->swordIdleAnimation;

		// Set texture
		this->handsSprite.setTexture(this->swordTextureSheet);
	}
}

void Player::updateFpsSpritePosition()
{
	this->swordPosition = sf::Vector2f(0, 540 - 64 * SWORD_SPRITE_SCALE / 2 + 30);
	this->swordPosition.x += sin(walkTimer * 7.0f) * 50;
	this->swordPosition.y += sin(walkTimer * 7.0f * 2.0f) * 30;

	// Regular position if the grenade animation is playing
	if (this->currentFpsAnimation == &this->grenadeThrowAnimation)
		this->swordPosition = sf::Vector2f(0, 540 - 64 * SWORD_SPRITE_SCALE / 2);

	// Move to fit screen
	ResTranslator::transformSprite(
		this->handsSprite,
		this->swordPosition.x,
		this->swordPosition.y,
		192 * SWORD_SPRITE_SCALE,
		64 * SWORD_SPRITE_SCALE
	);
	ResTranslator::transformSprite(
		this->berserkSprite,
		0,
		0,
		(float) BERSERK_SPRITE_WIDTH / BERSERK_SPRITE_HEIGHT * 1080,
		1080
	);

	// Set texture rects to current animation rects
	this->handsSprite.setTextureRect(this->currentFpsAnimation->getCurrentRect());
	this->berserkSprite.setTextureRect(this->currentBerserkAnimation->getCurrentRect());
}

Player::Player(int x, int y, EntityHandler& entityHandler)
	: x(x), y(y), entityHandler(entityHandler), lastFrameX(x), lastFrameY(y), direction(0.0f), walkTimer(0.0f), 
	isAttackingTimer(0.0f), tryToExit(false), hasStartedAttackAnimation(false), startThrowAnimation(false),
	hasSpawnedGrenade(false), currentFpsAnimation(&swordIdleAnimation), 
	currentBerserkAnimation(&startBerserkAnimation), grenade(nullptr)
{
	this->monitorMiddle = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);

	sf::Mouse::setPosition(this->monitorMiddle);

	// Load texture
	this->startBerserkTextureSheet.loadFromFile("Resources/Textures/armorCloseAnimation2.png");
	this->endBerserkTextureSheet.loadFromFile("Resources/Textures/armorOpenAnimation.png");
	this->grenadeThrowTextureSheet.loadFromFile("Resources/Textures/firstPersonThrowTextures.png");
	this->swordTextureSheet.loadFromFile("Resources/Textures/firstPersonSwordTexture.png");
	this->handsSprite.setTexture(this->swordTextureSheet);

	// Load animations
	this->loadAnimations();

	// Transform sprite
	this->updateFpsSpritePosition();
}

Player::~Player()
{
	delete this->grenade;
	delete this->grenadeExplosion;
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
	{
		// Reset cool down
		this->grenadeCooldownTimer = 0.0f;
		
		// Start animation
		this->startThrowAnimation = true;
	}

	// Berserker
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && this->berserkerCooldownTimer >= ABILITY_BERSERKER_MAX_COOLDOWN_TIME)
		this->berserkerCooldownTimer = 0.0f;

	// Walking
	float forwardInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	float rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

	sf::Vector2f forwardDir(std::cos(-this->direction), std::sin(-this->direction));
	sf::Vector2f rightDir(-forwardDir.y, forwardDir.x);
	walkStep = forwardDir * forwardInput + rightDir * rightInput;
	SMath::vectorNormalize(walkStep);

	if (SMath::dot(walkStep, walkStep) > 0.0f)
		this->walkTimer += deltaTime;
	else
		this->walkTimer = 0.0f;

	walkStep.x *= MOVEMENT_SPEED * deltaTime;
	walkStep.y *= MOVEMENT_SPEED * deltaTime;

	// Move position
	this->lastFrameX = this->x;
	this->lastFrameY = this->y;
	this->x += walkStep.x;
	this->y += walkStep.y;


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

	// Update grenade
	if (this->grenade)
	{
		this->grenade->update(deltaTime);

		// Get grenade explosion
		if (this->grenade->getGrenadeExplosion())
			this->grenadeExplosion = this->grenade->getGrenadeExplosion();

		// Grenade should be removed
		if (this->grenade->getShouldRemove())
		{
			delete this->grenade;
			this->grenade = nullptr;
		}
	}

	// Update grenadeExplosion
	if (this->grenadeExplosion)
	{
		this->grenadeExplosion->update(deltaTime);

		// Grenade explosion should be removed
		if (this->grenadeExplosion->getShouldRemove())
		{
			delete this->grenadeExplosion;
			this->grenadeExplosion = nullptr;
		}
	}

	// First person animations
	this->updateAnimationLogic(deltaTime);
	this->updateFpsSpritePosition();
}

void Player::render(sf::RenderWindow& window)
{
	window.draw(this->handsSprite);
	window.draw(this->berserkSprite);
}

void Player::setPlayerPosition(sf::Vector2f newPos)
{
	this->x = newPos.x;
	this->y = newPos.y;
}

const bool Player::playerTriesToExit() const
{
	return this->tryToExit;
}

const sf::Vector2f Player::getPlayerPosition() const
{
	return sf::Vector2f(x, y);
}

const sf::Vector2f Player::getPlayerWalkStep() const
{
	return walkStep;
}

const sf::Vector2f Player::getPlayerLastFramePosition() const
{
	return sf::Vector2f(lastFrameX, lastFrameY);
}

const sf::Glsl::Vec3 Player::getOrientation() const
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

float Player::getPlayerCollisionBoxSize() const
{
	return 0.15f;
}

Grenade* Player::getGrenade() const
{
	return this->grenade;
}

GrenadeExplosion* Player::getGrenadeExplosion() const
{
	return this->grenadeExplosion;
}
