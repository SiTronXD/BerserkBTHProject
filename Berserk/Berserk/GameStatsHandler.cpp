#include "GameStatsHandler.h"

GameStatsHandler::GameStatsHandler()
	: playerWon(false), playTimer(0.0f), numCollectedCollectibles(0), numMaxCollectibles(0)
{ }

void GameStatsHandler::reset()
{
	this->playerWon = false;
	this->numCollectedCollectibles = 0;
}

void GameStatsHandler::flagPlayerWon()
{
	this->playerWon = true;
}

void GameStatsHandler::foundCollectible()
{
	this->numCollectedCollectibles++;
}

void GameStatsHandler::setMaxCollectibles(int num)
{
	this->numMaxCollectibles = num;
}

void GameStatsHandler::updateTimer(float deltaTime)
{
	this->playTimer += deltaTime;
}

const bool GameStatsHandler::getPlayerHasWon() const
{
	return this->playerWon;
}

const int GameStatsHandler::getNumCollected() const
{
	return this->numCollectedCollectibles;
}

const int GameStatsHandler::getMaxNumCollectibles() const
{
	return this->numMaxCollectibles;
}

const int GameStatsHandler::getPlayTimeInSeconds() const
{
	return (int) this->playTimer;
}
