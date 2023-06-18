#include "Game.h"
// Private functions

void Game::initVariables()
{
    this->window = nullptr;
    this->gameState = GameState::StartScreen;

    // Game logic
    this->points = 0;
    this->health = 5;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
    this->mouseHeld = false;
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Buzo", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(144);
}

void Game::initFonts()
{
    if (!font.loadFromFile("assets/fonts/PressStart2P.ttf"))
    {
        // Failed to load the desired font, fall back to a random font
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cerr << "Failed to load fallback font." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Game::initStartScreen()
{
    // Initialize the title text
    this->titleText.setFont(font);
    this->titleText.setString("Buzo"); // Set the title text
    this->titleText.setCharacterSize(80);
    this->titleText.setFillColor(sf::Color::White);
    this->titleText.setStyle(sf::Text::Bold);
    this->titleText.setPosition(400.f, 200.f); // Set the position of the title text

    titlePos.x = window->getSize().x / 2.0f - titleText.getLocalBounds().width / 2.f;
    titlePos.y = 100.f; // Adjust the y-coordinate based on your desired position

    // Initialize the play button
    this->playButton.setSize(sf::Vector2f(100.f, 40.f));
    this->playButton.setPosition(350.f, 300.f);
    this->playButton.setFillColor(sf::Color::Black);
    this->playButton.setOutlineColor(sf::Color::White);
    this->playButton.setOutlineThickness(2.f);

    playButtonClicked = false;

    // Initialize the play text
    this->playText.setFont(font);
    this->playText.setString("Play");
    this->playText.setCharacterSize(24);
    this->playText.setFillColor(sf::Color::White); // Set the initial color to white
    this->playText.setPosition(353.f, 310.f);      // Adjust position if needed
}

void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Red);
    this->enemy.setOutlineColor(sf::Color::White);
    this->enemy.setOutlineThickness(4.f);
}

// Constructors / Destructors
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initEnemies();
    this->initFonts();
    this->initStartScreen();
}

Game::~Game()
{
    delete this->window;
}

// Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

////////////////////////////////////////////////////////////
// Functions ///////////////////////////////////////////////
////////////////////////////////////////////////////////////

////////////////
// Game State //
////////////////

void Game::setGameState(GameState state)
{
    gameState = state;
}

Game::GameState Game::getGameState() const
{
    return gameState;
}

/////////////
// Polling //
/////////////

void Game::pollEvents()
{
    // Event Polling
    while (this->window->pollEvent(this->ev))
    {
        if (this->gameState == GameState::StartScreen)
        {
            // Process events specific to the start screen
            switch (this->ev.type)
            {
            // If the window closes
            case sf::Event::Closed:
                this->window->close();
                break;
                // If escape is pressed to exit
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
            // If mouse clicks on play button
            case sf::Event::MouseButtonPressed:
                handleMouseButtonPressed();
                break;
            // If mouse hovers over play button
            case sf::Event::MouseMoved:
                handleMouseMoved();
                break;
            }
        }
        else if (this->gameState == GameState::InGame)
        {
            // Process events specific to the game
            switch (this->ev.type)
            {
            // If the window closes
            case sf::Event::Closed:
                this->window->close();
                break;
            // If escape is pressed to exit
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
            }
        }
    }
}

void Game::handleMouseButtonPressed()
{
    if (this->gameState == GameState::StartScreen && this->ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->window);
        sf::Vector2f mousePosView = this->window->mapPixelToCoords(mousePosWindow);

        if (getPlayButton().getGlobalBounds().contains(mousePosView))
        {
            // Change the title color and remove play button
            titleText.setFillColor(sf::Color::Red);
            titleText.setOutlineColor(sf::Color::White);
            titleText.setOutlineThickness(2.f);
            playButtonClicked = true;
            renderStartScreen();

            // Slide down the title text
            while (titlePos.y < window->getSize().y / 2.0f - titleText.getLocalBounds().height / 2.0f)
            {
                titlePos.y += 1.0f; // Adjust the sliding speed as needed
                renderStartScreen();
            }

            // Pause for 2 seconds
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            setGameState(GameState::InGame);
        }
    }
}

void Game::handleMouseMoved()
{
    if (this->gameState == GameState::StartScreen)
    {
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->window);
        sf::Vector2f mousePosView = this->window->mapPixelToCoords(mousePosWindow);

        if (getPlayButton().getGlobalBounds().contains(mousePosView))
        {
            // Change the color of the text and fill of the play button when hovered
            playButton.setFillColor(sf::Color::White);
            playText.setFillColor(sf::Color::Black);
        }
        else
        {
            // Reset the colors when not hovered
            playButton.setFillColor(sf::Color::Black);
            playText.setFillColor(sf::Color::White);
        }
    }
}

//////////////
// Spawning //
//////////////

void Game::spawnEnemy()
{
    /*
        @ return void

        Spawns enemies and sets their colors and positions.
        -Sets a random postion.
        -Adds enemy to the vector
    */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f);

    // Spawn the enemy
    this->enemies.push_back(this->enemy);

    // Remove enemies at the end of the screen
}

//////////////////
// Start Screen //
//////////////////

const sf::RectangleShape &Game::getPlayButton() const
{
    return playButton; // Return a reference to the play button rectangle
}

sf::Text &Game::getPlayText()
{
    return playText;
}

sf::Text &Game::getTitleText()
{
    return titleText;
}

///////////////
// Rendering //
///////////////

void Game::renderEnemies()
{
    for (auto &e : this->enemies)
    {
        this->window->draw(e);
    }
}

void Game::renderStartScreen()
{
    this->window->clear();
    // Draw the start screen objects (e.g., title, instructions, etc.)
    // using the window's draw function

    // Set the position of the title text
    titleText.setPosition(titlePos);

    // Render the title text
    this->window->draw(this->getTitleText());

    // Check if play button should be visible
    if (!playButtonClicked)
    {
        // Draw the play button
        window->draw(getPlayButton());

        // Draw the play text
        window->draw(getPlayText());
    }

    // Display the frame in the window
    this->window->display();
}

void Game::render()
{
    /*
        @return void
        Renders the game objects

        - clear old frame
        - render objects
        -display frame in window
    */
    this->window->clear();

    // Draw game objects
    this->renderEnemies();

    this->window->display();
}

//////////////
// Updating //
//////////////

void Game::updateEnemies()
{
    /*
        @return void

        Updates the enemy spawn timer and spawns the enemies
        when the total amount of enemies is smaller than the maximum.
        Moves the enemies downwards.
        Removes the enemies at the edge of the screen
    */

    // Updating the timer for enemy spawning

    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            // Spawn the enemy and reset the timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    // Move and update enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;

        this->enemies[i].move(0.f, 1.f);

        // Delete enemy when it goes off-screen
        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "Health: " << this->health << std::endl;
        }

        // Check if an enemy is clicked and delete it
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (this->mouseHeld == false)
            {
                this->mouseHeld = true;
                bool deleted = false;
                for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
                {
                    if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                    {
                        // Delete the enemy
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);

                        // Gain points
                        this->points += 10;
                        std::cout << "Points: " << this->points << std::endl;
                    }
                }
            }
        }
        else
        {
            this->mouseHeld = false;
        }
    }
}

void Game::updateMousePos()
{
    /*
        @ return void

        Updates the mouse positions:
        -Mouse position relative to window (Vesctor2i)
    */

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateStartScreen()
{
    this->pollEvents();

    this->updateMousePos();
}

void Game::update()
{
    this->pollEvents();

    this->updateMousePos();

    this->updateEnemies();
}