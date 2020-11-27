#include "GameStatsHandler.h"

GameStatsHandler::GameStatsHandler()
	: playerWon(false)
{ }

void GameStatsHandler::flagPlayerWon()
{
	this->playerWon = true;
}

const bool GameStatsHandler::getPlayerHasWon() const
{
	return this->playerWon;
}
