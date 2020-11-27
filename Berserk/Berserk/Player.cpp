#include "Player.h"

void Player::updateSwordPosition()
{
	this->swordPosition = sf::Vector2f(0, 540 - 64 * SWORD_SPRITE_SCALE / 2 + 30);
	this->swordPosition.x += sin(walkTimer * 7.0f) * 50;
	this->swordPosition.y += sin(walkTimer * 7.0f * 2.0f) * 30;

	ResTranslator::transformSprite(
		this->swordSprite,
		this->swordPosition.x,
		this->swordPosition.y,
		192 * SWORD_SPRITE_SCALE,
		64 * SWORD_SPRITE_SCALE
	);
}

Player::Player(int x, int y)
	: x(x), y(y), direction(0.0f), walkTimer(0.0f), tryToExit(false)
{
	this->monitorMiddle = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);

	sf::Mouse::setPosition(this->monitorMiddle);

	// Load texture
	this->swordTextureSheet.loadFromFile("Resources/Textures/firstPersonSwordTexture.png");
	this->swordSprite.setTexture(this->swordTextureSheet);
	this->swordSprite.setTextureRect(sf::IntRect(0, 0, 192, 64));

	// Transform sprite
	this->updateSwordPosition();
}

void Player::handleInput(float deltaTime)
{
	// Try to exit
	this->tryToExit = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);

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