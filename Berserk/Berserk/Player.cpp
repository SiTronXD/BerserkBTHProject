#include "Player.h"

Player::Player(int x, int y)
	: x(x), y(y), direction(3.1415f * 0.5f), lastMouseX(0.0f)
{
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

	// Mouse look
	this->direction += (lastMouseX - sf::Mouse::getPosition().x) * 0.001f * SettingsHandler::getMouseSensitivity();

	// Move mouse to the middle of the monitor
	sf::Mouse::setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2));
	this->lastMouseX = sf::Mouse::getPosition().x;
}

const sf::Glsl::Vec3 Player::getPlayerCamera() const
{
	return sf::Glsl::Vec3(x, y, direction);
}