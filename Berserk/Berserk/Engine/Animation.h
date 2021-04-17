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
	void initMove(Animation& other);

public:
	Animation();
	Animation(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat);
	Animation(const Animation &other);
	Animation(Animation&& other) noexcept;
	~Animation();
	Animation& operator=(const Animation& other);
	Animation& operator=(Animation&& other) noexcept;

	void init(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat);
	void update(float deltaTime);
	void reset();

	const sf::IntRect& getCurrentRect() const;
	bool isDone() const;
	int getCurrentRectIndex() const;
	int getLastFrameRectIndex() const;
};