#include "Player.h"

Player::Player(int x, int y)
	: x(x), y(y), direction(0.0f)
{
	this->monitorMiddle = sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2);

	sf::Mouse::setPosition(this->monitorMiddle);
}

void Player::handleInput(float deltaTime)
{
	// Walking
	float forwardInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	float rightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

	sf::Vector2f forwardDir(std::cos(-this->direction), std::sin(-this->direction));
	sf::Vector2f rightDir(-forwardDir.y, forwardDir.x);
	sf::Vector2f walkStep = forwardDir * forwardInput + rightDir * rightInput;
	SMath::vectorNormalize(walkStep);

	this->x += walkStep.x * MOVEMENT_SPEED * deltaTime;
	this->y += walkStep.y * MOVEMENT_SPEED * deltaTime;

	// Keyboard to turn the player
	this->direction += (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) * 0.001f * SettingsHandler::getKeyboardLookSensitivity();
	
	// Mouse look
	this->direction += (this->monitorMiddle.x - sf::Mouse::getPosition().x) * 0.001f * SettingsHandler::getMouseSensitivity();

	// Move mouse to the middle of the monitor
	sf::Mouse::setPosition(this->monitorMiddle);
}

const sf::Glsl::Vec3 Player::getPlayerCamera() const
{
	return sf::Glsl::Vec3(x, y, direction);
}