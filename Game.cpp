#include "Game.h"
// Private functions
void Game::initVariables()
{
    this->window = nullptr;
    this->gameState = GameState::StartScreen;
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Buzo", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(144);

    // Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 1000.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;

    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        // Failed to load the desired font, fall back to a random font
        if (!font.loadFromFile("arial.ttf"))
        {
        std::cerr << "Failed to load fallback font." << std::endl;
        exit(EXIT_FAILURE); // or handle the error as needed
        }
    }
}

void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    this->enemy.setOutlineColor(sf::Color::Green);
    this->enemy.setOutlineThickness(1.f);
}

// Constructors / Destructors
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initEnemies();
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
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            else if (this->gameState == GameState::StartScreen && this->ev.key.code == sf::Keyboard::Space)
                this->gameState = GameState::InGame;
            break;
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
        -Sets a random color.
        -Adds enemy to the vector
    */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    this->enemy.setFillColor(sf::Color::Green);

    //Spawn the enemy
    this->enemies.push_back(this->enemy);

    //Remove enemies at the end of the screen
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

     // Create a title text object
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Buzo");  // Set the title text
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(300.f, 200.f);  // Set the position of the title text

    // Render the title text
    this->window->draw(titleText);

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

    //Updating the timer for enemy spawing
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            //Spawn the enemy and reset the timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    //Move the enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, 1.f);

        //Check if clicked upon
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
            {
                this->enemies.erase(this->enemies.begin() + i);
            }
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
    // Check for user input to start the game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        // Start the game by changing the game state
        setGameState(GameState::InGame);
    }
}

void Game::update()
{
    this->pollEvents();

    this->updateMousePos();

    this->updateEnemies();
}