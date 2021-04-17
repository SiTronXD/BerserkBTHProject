#include "SMath.h"

float SMath::sign(const float& val)
{
	if (val > 0.0f)
		return 1.0f;

	return -1.0f;
}

// Dot product between v1 and v2
float SMath::dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float SMath::dot(const sf::Vector3f& v1, const sf::Vector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Normalize vector into a unit vector
void SMath::vectorNormalize(sf::Vector2f& vector)
{
	float length = sqrt(dot(vector, vector));

	// Normalize the non-zero vector
	if (length != 0.0f)
		vector /= length;
}

sf::Vector2f SMath::lerp(const sf::Vector2f& v1, const sf::Vector2f& v2, const float& t)
{
	return v1 * (1.0f - t) + v2 * t;
}
