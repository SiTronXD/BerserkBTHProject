#include "Animation.h"

Animation::Animation()
	: numTextureRects(0), textureRects(nullptr), frameTime(0), 
	currentTextureRectIndex(0), timer(0), repeat(true)
{ }

Animation::~Animation()
{
	delete[] this->textureRects;
}

void Animation::init(int numTextureRects, sf::IntRect tr[], float frameTime, bool repeat)
{
	this->numTextureRects = numTextureRects;
	this->frameTime = frameTime;
	this->repeat = repeat;

	// Deep allocate textureRects
	this->textureRects = new sf::IntRect[this->numTextureRects];
	for (unsigned int i = 0; i < this->numTextureRects; ++i)
	{
		this->textureRects[i] = sf::IntRect(tr[i]);
	}

	this->currentTextureRectIndex = 0;
	this->timer = 0;
}

void Animation::update(float deltaTime)
{
	this->timer += deltaTime;
	
	// Switch frame
	if (this->timer >= this->frameTime)
	{
		this->timer -= this->frameTime;
		this->currentTextureRectIndex++;

		// Loop to the beginning
		if (this->currentTextureRectIndex >= this->numTextureRects)
		{
			if (this->repeat)
				this->currentTextureRectIndex = 0;
			else
				this->currentTextureRectIndex = this->numTextureRects;	// Off by one since the last frame should play thoughout it's own lifespan
		}
	}
}

void Animation::reset()
{
	this->currentTextureRectIndex = 0;
	this->timer = 0.0f;
}

const sf::IntRect& Animation::getCurrentRect() const
{
	int index = this->currentTextureRectIndex;
	index = std::min(index, this->numTextureRects-1);

	return this->textureRects[index];
}

bool Animation::isDone() const
{
	return !repeat && this->currentTextureRectIndex >= this->numTextureRects;
}
