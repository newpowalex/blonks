#pragma once

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

/*
    Class that acts as the game engine.
    Wrapper class.
*/
class Game
{
    private:
    //Variables

    //Resources
    sf::Font font;

    //Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    //Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Game logic
    int points;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;

    //Start Screen Objects
    bool playButtonClicked;
    sf::RectangleShape playButton;
    sf::Text titleText;
    sf::Vector2f titlePos;
    sf::Text playText;

    //Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    //Private functions
    void initVariables();
    void initWindow();
    void initEnemies();
    void initFonts();
    void initStartScreen();


    public:
        //Constructors / Destructors
        Game();
        virtual ~Game();

        //Accessors
        const bool running() const;

        //Functions
        //Game State
        enum class GameState
        {
            StartScreen,
            InGame
        };

        //Game State
        GameState gameState;

        GameState getGameState() const;
        void setGameState(GameState state);

        //Polling
        void pollEvents();

        //Rendering
        void renderEnemies();
        void renderStartScreen();
        void render();

        //Spawning
        void spawnEnemy();

        //Start Screen
        const sf::RectangleShape& getPlayButton() const;
        sf::Text& getPlayText();
        sf::Text& getTitleText();
        void handleMouseButtonPressed();
        void handleMouseMoved();
        
        //Updating
        void updateEnemies();
        void updateMousePos();
        void updateStartScreen();
        void update();

        
};