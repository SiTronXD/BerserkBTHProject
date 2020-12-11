#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
private:
	sf::IntRect* textureRects;

	int numTextureRects;
	int currentTextureRectIndex;
	int lastFrameTextureRectIndex;

	float frameTime;
	float timer;

	bool repeat;

	void cleanUpMemory();

public:
	Animation();
	Animation(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat);
	Animation(const Animation &other);
	~Animation();
	Animation& operator=(const Animation& other);

	void init(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat);
	void update(float deltaTime);
	void reset();

	const sf::IntRect& getCurrentRect() const;
	bool isDone() const;
	int getCurrentRectIndex() const;
	int getLastFrameRectIndex() const;
};