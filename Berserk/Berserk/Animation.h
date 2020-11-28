#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
private:
	sf::IntRect* textureRects;

	int numTextureRects;
	int currentTextureRectIndex;

	float frameTime;
	float timer;

	bool repeat;

public:
	Animation();
	~Animation();

	void init(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat);
	void update(float deltaTime);
	void reset();

	const sf::IntRect& getCurrentRect() const;
	bool isDone() const;
};