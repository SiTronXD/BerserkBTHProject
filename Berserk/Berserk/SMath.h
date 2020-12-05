#pragma once

#include <SFML/Graphics.hpp>

class SMath
{
private:

public:
	template <typename T>
	static T clamp(T val, T min, T max)
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;

		return val;
	}

	static float sign(const float& val);
	static float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
	static float dot(const sf::Vector3f& v1, const sf::Vector3f& v2);
	static void vectorNormalize(sf::Vector2f& vector);
	static sf::Vector2f lerp(const sf::Vector2f& v1, const sf::Vector2f& v2, const float& t);
};