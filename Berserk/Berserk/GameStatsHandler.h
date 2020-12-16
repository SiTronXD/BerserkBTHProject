#pragma once

class GameStatsHandler
{
private:
	bool playerWon;

	float playTimer;

	int numCollectedCollectibles;
	int numMaxCollectibles;
	int numKilledEnemies;
	int numMaxEnemies;

	GameStatsHandler(const GameStatsHandler& other) = delete;
	GameStatsHandler operator=(const GameStatsHandler& other) = delete;

public:
	GameStatsHandler();

	void reset();
	void flagPlayerWon();
	void foundCollectible();
	void setMaxCollectibles(int num);
	void killedEnemy();
	void setMaxNumEnemies(int num);

	void updateTimer(float deltaTime);

	const bool getPlayerHasWon() const;
	const int getNumCollected() const;
	const int getMaxNumCollectibles() const;
	const int getPlayTimeInSeconds() const;
	const int getNumKilledEnemies() const;
	const int getMaxNumEnemies() const;
};