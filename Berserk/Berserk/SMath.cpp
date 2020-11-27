#include "SMath.h"

float SMath::dot(const sf::Color& c1, const sf::Color& c2)
{
	float r1 = c1.r / 255.0f;
	float g1 = c1.g / 255.0f;
	float b1 = c1.b / 255.0f;

	float r2 = c2.r / 255.0f;
	float g2 = c2.g / 255.0f;
	float b2 = c2.b / 255.0f;

	return r1 * r2 + g1 * g2 + b1 * b2;
}

// Dot product between v1 and v2
float SMath::dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// Normalize vector into a unit vector
void SMath::vectorNormalize(sf::Vector2f& vector)
{
	float length = sqrt(dot(vector, vector));

	// Normalize the non-zero vector
	if (length != 0.0f)
		vector /= length;
}