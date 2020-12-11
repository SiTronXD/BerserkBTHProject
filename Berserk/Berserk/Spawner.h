#pragma once

#include <SFML/Graphics.hpp>

class Spawner
{
private:


	Spawner(const Spawner& other) = delete;
	Spawner operator=(const Spawner& other) = delete;

public:
	Spawner();

	void addSpawnPoint(sf::Vector2f spawnPointPos);
};