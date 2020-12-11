#include "Animation.h"

void Animation::cleanUpMemory()
{
	delete[] this->textureRects;
}

Animation::Animation()
	: numTextureRects(0), textureRects(nullptr), frameTime(0), 
	currentTextureRectIndex(0), lastFrameTextureRectIndex(0), timer(0), repeat(true)
{ }

Animation::Animation(int numTextureRects, sf::IntRect textureRects[], float frameTime, bool repeat)
	: numTextureRects(0), textureRects(nullptr), frameTime(0),
	currentTextureRectIndex(0), lastFrameTextureRectIndex(0), timer(0), repeat(true)
{
	this->init(numTextureRects, textureRects, frameTime, repeat);
}

Animation::Animation(const Animation& other)
{
	this->init(other.numTextureRects, other.textureRects, other.frameTime, other.repeat);
}

Animation::~Animation()
{
	this->cleanUpMemory();
}

Animation& Animation::operator=(const Animation& other)
{
	this->cleanUpMemory();

	this->init(other.numTextureRects, other.textureRects, other.frameTime, other.repeat);

	return *this;
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
	
	this->lastFrameTextureRectIndex = this->currentTextureRectIndex;

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

int Animation::getCurrentRectIndex() const
{
	return this->currentTextureRectIndex;
}

int Animation::getLastFrameRectIndex() const
{
	return this->lastFrameTextureRectIndex;
}
