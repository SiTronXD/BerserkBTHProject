#include "Game.h"
#include "SettingsHandler.h"

void Game::handlePollEvent(const sf::Event& event)
{
}

void Game::update()
{
}

void Game::render(sf::RenderWindow& target)
{
    target.clear();
    target.draw(shape);
    target.display();
}

Game::Game()
    :   window(sf::VideoMode(SettingsHandler::getWidth(), SettingsHandler::getHeight()), "Berserk"),
        shape(100.f)
{
    shape.setFillColor(sf::Color::Green);
}

void Game::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            this->handlePollEvent(event);
        }

        this->update();
        this->render(window);
    }
}