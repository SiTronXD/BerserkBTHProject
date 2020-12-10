#include "Game.h"
#include "SettingsHandler.h"

void Game::handlePollEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            this->window.close();
    }

    if(this->currentState)
        this->currentState->handlePollEvent(event);
}

void Game::update()
{
    float deltaTime = deltaTimeClock.restart().asSeconds();

    // Change state if necessary
    if (this->currentState)
    {
        if (currentState->getSwitchToState() != State::NONE)
            setState(currentState->getSwitchToState());

        // Update state
        currentState->update(deltaTime);
    }

    // Update transition color
    if ((this->transitionAlpha > 0.0f && this->transitionDirection < 0) ||
        (this->transitionAlpha <= 1.0f && this->transitionDirection > 0))
    {
        this->transitionAlpha += this->transitionDirection * TRANSITION_SPEED_SCALE * deltaTime;
        this->transitionAlpha = SMath::clamp(this->transitionAlpha, 0.0f, 1.0f);

        // Switch state
        if (this->transitionAlpha >= 1.0f && this->transitionDirection > 0)
        {
            this->applyNewState();

            this->transitionDirection = -1;
        }

        // Apply color
        sf::Color newTransitionColor(255, 255, 255, (sf::Uint8) (255 * this->transitionAlpha));
        this->transitionSprite.setColor(newTransitionColor);
    }
}

void Game::render()
{
    this->window.clear();

    if(this->currentState)
        currentState->render();
    
    this->window.draw(this->transitionSprite);

    this->window.display();
}

void Game::setState(State newState)
{
    this->stateToSwitchTo = newState;

    this->transitionDirection = 1;
}

void Game::applyNewState()
{
    delete currentState;

    switch (this->stateToSwitchTo)
    {
    case State::MAIN_MENU:
        currentState = new MainMenuState(window);
        break;
    case State::PLAY:
        currentState = new PlayState(window, gameStats);
        break;
    case State::GAME_OVER:
        currentState = new GameOverState(window, gameStats);
        break;
    default:
        std::cout << "CAN'T FIND PLAYSTATE" << std::endl;
        break;
    }
}

void Game::applySettings()
{
    SettingsHandler::loadSettings();

    sf::Uint32 windowStyle = SettingsHandler::getIsFullscreen() ?
        sf::Style::Fullscreen : (sf::Style::Titlebar | sf::Style::Close);

    // Create window with the new settings
    this->window.create(
        sf::VideoMode(SettingsHandler::getWidth(), SettingsHandler::getHeight()),
        "Berserk",
        windowStyle
    );
}

void Game::setWindowIcon()
{
    // Load image
    sf::Image iconImage;
    iconImage.loadFromFile("Resources/Textures/berserkIcon.png");

    // Apply image
    this->window.setIcon(32, 32, iconImage.getPixelsPtr());
}

void Game::setupTransition()
{
    this->blackBoxTexture.loadFromFile("Resources/Textures/BlackBox.png");
    this->transitionSprite.setTexture(this->blackBoxTexture);
    ResTranslator::transformSprite(
        this->transitionSprite, 
        0, 
        0, 
        (float) ResTranslator::getVirtualWidth(), 
        1080
    );
}

Game::Game()
    : currentState(nullptr),
    transitionAlpha(1.0f),
    transitionDirection(-1)
{
    srand((unsigned int) time(0));

    this->applySettings();
    this->setWindowIcon();
    this->setupTransition();
    this->setState(State::PLAY);
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