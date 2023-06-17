#include "Game.h"
// Private functions
void Game::initVariables()
{
    this->window = nullptr;
    this->gameState = GameState::StartScreen;

    // Initialize the play button
    this->playButton.setSize(sf::Vector2f(100.f, 40.f));
    this->playButton.setPosition(330.f, 300.f);
    this->playButton.setFillColor(sf::Color::Black);
    this->playButton.setOutlineColor(sf::Color::White);
    this->playButton.setOutlineThickness(2.f);
    
    // Initialize the play text
    this->playText.setFont(font);
    this->playText.setString("Play");
    this->playText.setCharacterSize(24);
    this->playText.setFillColor(sf::Color::White); // Set the initial color to white
    this->playText.setPosition(333.5f, 310.f); // Adjust position if needed
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Buzo", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(144);

    // Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 20.f;
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
            if (this->gameState == GameState::StartScreen && this->ev.mouseButton.button == sf::Mouse::Left)
            {
                // Get the mouse position relative to the window
                sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->window);

                // Convert the mouse position to view coordinates
                sf::Vector2f mousePosView = this->window->mapPixelToCoords(mousePosWindow);

                // Check if the mouse is inside the play button
                if (getPlayButton().getGlobalBounds().contains(mousePosView))
                {
                    // Start the game by changing the game state
                    setGameState(GameState::InGame);
                }
            }
            break;
        // If mouse hovers over play button
        case sf::Event::MouseMoved:
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
        -Adds enemy to the vector
    */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    //Spawn the enemy
    this->enemies.push_back(this->enemy);

    //Remove enemies at the end of the screen
}



//////////////////
// Start Screen //
//////////////////

const sf::RectangleShape& Game::getPlayButton() const
{
    return playButton; // Return a reference to the play button rectangle
}

sf::Text& Game::getPlayText()
{
    return playText;
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

    // Set the cursor to the arrow cursor
    this->window->setMouseCursorVisible(true);
    sf::Cursor cursor;
    if (cursor.loadFromSystem(sf::Cursor::Arrow))
    {
        this->window->setMouseCursor(cursor);
    }

     // Create a title text object
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Buzo");  // Set the title text
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(300.f, 200.f);  // Set the position of the title text

    // Render the play button and title text
    this->window->draw(titleText);
    this->window->draw(this->getPlayButton());
    this->window->draw(this->getPlayText());

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
    this->pollEvents();

    this->updateMousePos();

}

void Game::update()
{
    this->pollEvents();

    this->updateMousePos();

    this->updateEnemies();
}