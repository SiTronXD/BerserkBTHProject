#include "SMath.h"

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