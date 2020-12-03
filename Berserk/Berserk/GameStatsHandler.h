#pragma once

class GameStatsHandler
{
private:
	bool playerWon;

	float playTimer;

	int numCollectedCollectibles;
	int numMaxCollectibles;

public:
	GameStatsHandler();

	void reset();
	void flagPlayerWon();
	void foundCollectible();
	void setMaxCollectibles(int num);

	void updateTimer(float deltaTime);

	const bool getPlayerHasWon() const;
	const int getNumCollected() const;
	const int getMaxNumCollectibles() const;
	const int getPlayTimeInSeconds() const;
};