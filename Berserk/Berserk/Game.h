#include <SFML/Graphics.hpp>

class Game
{
private:
	sf::RenderWindow window;
	sf::CircleShape shape;

	void handlePollEvent(const sf::Event& event);
	void update();
	void render(sf::RenderWindow& target);

public:
	Game();

	void run();
};