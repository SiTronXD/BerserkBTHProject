#pragma once

#include <SFML/Graphics.hpp>

class SMath
{
private:

public:
	static float dot(const sf::Color& c1, const sf::Color& c2);
	static float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
	static void vectorNormalize(sf::Vector2f& vector);

};