#include "Game.h"
#include "SettingsHandler.h"

void Game::handlePollEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            this->window.close();
    }

    currentState->handlePollEvent(event);
}

void Game::update()
{
    // Change state if necessary
    if(currentState->getSwitchToState() != State::NONE)
        setState(currentState->getSwitchToState());

    // Update
    float deltaTime = deltaTimeClock.restart().asSeconds();
    currentState->update(deltaTime);
}

void Game::render()
{
    this->window.clear();

    currentState->render();

    this->window.display();
}

void Game::setState(State newState)
{
    delete currentState;

    switch (newState)
    {
    case State::MAIN_MENU:
        currentState = new MainMenuState(window);
            break;
    case State::PLAY:
        currentState = new PlayState(window);
        break;
    default:
        std::cout << "CAN'T FIND PLAYSTATE" << std::endl;
        break;
    }
}

Game::Game()
    :   window(
            sf::VideoMode(SettingsHandler::getWidth(), SettingsHandler::getHeight()), 
            "Berserk", 
            sf::Style::Titlebar | sf::Style::Close
        ),
        currentState(nullptr)
{
    this->setState(State::MAIN_MENU);
}

Game::~Game()
{
    delete currentState;
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
        this->render();
    }
}