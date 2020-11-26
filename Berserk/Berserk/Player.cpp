#include "Player.h"

Player::Player(int x, int y)
	: x(x), y(y), direction(3.1415f * 0.5f)
{
}

void Player::handleInput(float deltaTime)
{
	this->x += deltaTime;
}

void Player::addDirection(float deltaDirection)
{
	this->direction += deltaDirection;
}

const sf::Glsl::Vec3 Player::getPlayerCamera() const
{
	return sf::Glsl::Vec3(x, y, direction);
}