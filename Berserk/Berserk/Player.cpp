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
		0.08f,
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
		0.08f,
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
		0.08f,
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

	// Place out and spawn
	sf::Vector2f dirVec = sf::Vector2f(cos(this->direction), -sin(this->direction));
	sf::Vector2f spawnPos = this->getPosition() + dirVec;
	this->grenade = new Grenade(spawnPos, dirVec);
}

void Player::updateAnimationLogic(float deltaTime)
{
	this->isAttackingTimer -= deltaTime;
	this->isAttackingTimer = std::max(this->isAttackingTimer, 0.0f);

	// Update animations
	this->currentFpsAnimation->update(deltaTime);

	if(this->currentBerserkAnimation && this->berserkerAnimationAlpha >= 0.5f)
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

	// Start start berserk animation
	if (this->startStartBerserkAnimation && this->currentBerserkAnimation != &this->startBerserkAnimation)
	{
		this->startStartBerserkAnimation = false;

		this->currentBerserkAnimation = &this->startBerserkAnimation;
		this->currentBerserkAnimation->reset();

		this->berserkSprite.setTexture(startBerserkTextureSheet);
	}

	// Start end berserk animation
	if (this->startEndBerserkAnimation && 
		this->currentBerserkAnimation == &this->startBerserkAnimation)
	{
		this->startEndBerserkAnimation = false;

		this->currentBerserkAnimation = &this->endBerserkAnimation;
		this->currentBerserkAnimation->reset();

		this->berserkSprite.setTexture(endBerserkTextureSheet);
	}
	// Remove animation if it is done
	else if (this->currentBerserkAnimation == &this->endBerserkAnimation &&
		this->currentBerserkAnimation->isDone())
	{
		this->currentBerserkAnimation = nullptr;
	}

	// Activate berserker powerups
	if (this->currentBerserkAnimation == &this->startBerserkAnimation &&
		this->currentBerserkAnimation->getCurrentRectIndex() >= 2)
	{
		this->berserkerIsActive = true;
	}

	// Deactivate berserker powerups
	if (this->currentBerserkAnimation == &this->endBerserkAnimation &&
		this->currentBerserkAnimation->getCurrentRectIndex() >= 2)
	{
		this->berserkerIsActive = false;
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
		1923,
		1080
	);

	// Set texture rects to current animation rects
	this->handsSprite.setTextureRect(this->currentFpsAnimation->getCurrentRect());


	// Update berserker alpha color
	sf::Color newSpriteColor = sf::Color(255, 255, 255, 255 * berserkerAnimationAlpha);
	this->berserkSprite.setColor(newSpriteColor);

	// Set texture rects
	if(this->currentBerserkAnimation)
		this->berserkSprite.setTextureRect(this->currentBerserkAnimation->getCurrentRect());
}

Player::Player(int x, int y, EntityHandler& entityHandler)
	: x(x), y(y), z(0.0f), entityHandler(entityHandler), lastFrameX(x), lastFrameY(y), 
	direction(0.0f), walkTimer(0.0f), isAttackingTimer(0.0f), berserkerAnimationAlpha(1.0f), dieTimer(0.0f),
	health(100.0f), tryToExit(false), hasStartedAttackAnimation(false), startThrowAnimation(false),
	hasSpawnedGrenade(false), currentFpsAnimation(&swordIdleAnimation), 
	currentBerserkAnimation(nullptr), grenade(nullptr)
{
	this->monitorMiddle = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);

	sf::Mouse::setPosition(this->monitorMiddle);

	// Load textures
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
		// Start timer
		this->isAttackingTimer = this->MAX_ATTACK_TIME;

		// Start animation
		this->hasStartedAttackAnimation = false;

		// Set direction
		this->walkStep = sf::Vector2f(cos(direction), -sin(direction));
	}

	// Grenade
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && 
		this->grenadeCooldownTimer >= ABILITY_GRENADE_MAX_COOLDOWN_TIME)
	{
		// Reset cooldown
		this->grenadeCooldownTimer = 0.0f;
		
		// Start animation
		this->startThrowAnimation = true;
	}

	// Berserker
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && 
		this->berserkerCooldownTimer >= ABILITY_BERSERKER_MAX_COOLDOWN_TIME)
	{
		// Reset cooldown
		this->berserkerCooldownTimer = 0.0f;

		// Start ability timer
		this->berserkerActiveTimer = MAX_BERSERKER_TIME;

		// Start animation
		this->startStartBerserkAnimation = true;
	}

	// Walking
	float forwardInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	float rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

	sf::Vector2f forwardDir(std::cos(-this->direction), std::sin(-this->direction));
	sf::Vector2f rightDir(-forwardDir.y, forwardDir.x);

	// Update the direction if the player is not attacking
	if (this->isAttackingTimer <= 0.0f)
		this->walkStep = forwardDir * forwardInput + rightDir * rightInput;

	SMath::vectorNormalize(this->walkStep);

	// Is the player walking?
	if (SMath::dot(this->walkStep, this->walkStep) > 0.0f)
		this->walkTimer += deltaTime;
	else
		this->walkTimer = 0.0f;

	// Set current movement speed
	float currentMovementSpeed = MOVEMENT_SPEED_DEFAULT;
	if (this->berserkerIsActive)
		currentMovementSpeed = MOVEMENT_SPEED_BERSERKER;

	// Move faster if the player is attacking
	if (this->isAttackingTimer > 0.0f)
		currentMovementSpeed *= MOVEMENT_SPEED_ATTACKING_SCALE;

	// Apply speed
	this->walkStep.x *= currentMovementSpeed * deltaTime;
	this->walkStep.y *= currentMovementSpeed * deltaTime;

	// Move position
	this->lastFrameX = this->x;
	this->lastFrameY = this->y;
	this->x += this->walkStep.x;
	this->y += this->walkStep.y;

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
	this->berserkerActiveTimer -= deltaTime;
	this->grenadeCooldownTimer = SMath::clamp(this->grenadeCooldownTimer, 0.0f, ABILITY_GRENADE_MAX_COOLDOWN_TIME);
	this->berserkerCooldownTimer = SMath::clamp(this->berserkerCooldownTimer, 0.0f, ABILITY_BERSERKER_MAX_COOLDOWN_TIME);
	this->berserkerActiveTimer = std::max(this->berserkerActiveTimer, 0.0f);

	// Update open berserker timer
	if (this->berserkerActiveTimer <= 0.0f && !this->startEndBerserkAnimation && 
		this->currentBerserkAnimation == &this->startBerserkAnimation)
	{
		this->startEndBerserkAnimation = true;
	}

	// Update berserker ended timer
	if (this->currentBerserkAnimation == &this->startBerserkAnimation && 
		this->currentBerserkAnimation->isDone())
	{
		this->berserkerAnimationAlpha -= deltaTime * BERSERKER_ALPHA_ANIMATION_TIME_SCALE;
		this->berserkerAnimationAlpha = SMath::clamp(this->berserkerAnimationAlpha, 0.0f, 1.0f);
	}
	else if(this->currentBerserkAnimation == &this->endBerserkAnimation && this->berserkerAnimationAlpha < 1.0f)
	{
		this->berserkerAnimationAlpha += deltaTime * BERSERKER_ALPHA_ANIMATION_TIME_SCALE;
		this->berserkerAnimationAlpha = SMath::clamp(this->berserkerAnimationAlpha, 0.0f, 1.0f);
	}

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

	// Update die timer
	if (this->dead)
	{
		this->dieTimer += deltaTime;
		this->dieTimer = SMath::clamp(
			this->dieTimer, 
			0.0f, 
			this->MAX_DIE_ANIMATION_TIME + this->MAX_POST_DIE_ANIMATION_TIME
		);

		// Player vertical position animation
		float tempX = SMath::clamp(this->dieTimer / this->MAX_DIE_ANIMATION_TIME, 0.0f, 1.0f) * 0.5f;
		float s1 = std::max(cos(tempX * 6.0f), 0.0f);
		float s2 = std::max(cos(tempX * 10.0f + 3.0f), 0.0f) * 0.6f;
		this->z = std::max(s1, s2) * 0.4f - 0.4f;

		// Player roll
		this->roll = -std::pow(tempX * 2.0f, 2.2f);
	}

	// First person animations
	this->updateAnimationLogic(deltaTime);
	this->updateFpsSpritePosition();
}

void Player::render(sf::RenderWindow& window)
{
	if (!this->dead)
	{
		// Hands
		window.draw(this->handsSprite);

		// Berserker armor
		if (this->currentBerserkAnimation)
			window.draw(this->berserkSprite);
	}
}

void Player::setPosition(sf::Vector2f newPos)
{
	this->x = newPos.x;
	this->y = newPos.y;
}

void Player::gainHealth()
{
	this->health += HP_INCREASE_AMOUNT;
	this->health = std::min(this->health, 100.0f);
}

void Player::loseHealth()
{
	this->health -= HP_DECREASE_AMOUNT;
	this->health = std::max(this->health, 0.0f);

	// Flag dead
	if (this->health <= 0.0f)
		this->dead = true;
}

const bool Player::playerTriesToExit() const
{
	return this->tryToExit;
}

const sf::Vector2f Player::getPosition() const
{
	return sf::Vector2f(x, y);
}

const sf::Vector2f Player::getWalkStep() const
{
	return walkStep;
}

const sf::Vector2f Player::getLastFramePosition() const
{
	return sf::Vector2f(lastFrameX, lastFrameY);
}

const sf::Vector2f Player::getLookDirectionVec() const
{
	return sf::Vector2f(std::cos(this->direction), -std::sin(this->direction));
}

const sf::Glsl::Vec3 Player::getPositionForRenderer() const
{
	return sf::Glsl::Vec3(x, y, z);
}

const sf::Glsl::Vec2 Player::getRotationForRenderer() const
{
	return sf::Glsl::Vec2(direction, roll);
}

bool Player::isAttacking() const
{
	return this->currentFpsAnimation == &this->swordAttackAnimation;
}

bool Player::isBerserkerActive() const
{
	return this->berserkerIsActive;
}

bool Player::isHealthDepleted() const
{
	return this->health <= 0;
}

bool Player::isDead() const
{
	return this->isHealthDepleted() && 
		this->dieTimer >= this->MAX_DIE_ANIMATION_TIME + this->MAX_POST_DIE_ANIMATION_TIME;
}

int Player::getCurrentHealth() const
{
	return (int) this->health;
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

float Player::getAttackConeAngle() const
{
	return this->ATTACK_CONE_ANGLE;
}

float Player::getMaxAttackDistSqrd() const
{
	return this->MAX_ATTACK_DIST * this->MAX_ATTACK_DIST;
}

float Player::getBerserkerBlackBarAlpha() const
{
	float alpha = this->berserkerAnimationAlpha;

	if (!this->currentBerserkAnimation)
		alpha = 0.0f;

	return alpha;
}

Grenade* Player::getGrenade() const
{
	return this->grenade;
}

GrenadeExplosion* Player::getGrenadeExplosion() const
{
	return this->grenadeExplosion;
}
