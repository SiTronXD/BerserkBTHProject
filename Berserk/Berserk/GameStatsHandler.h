#pragma once

class GameStatsHandler
{
private:
	bool playerWon;

public:
	GameStatsHandler();

	void flagPlayerWon();

	const bool getPlayerHasWon() const;
};