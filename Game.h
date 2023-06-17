#pragma once

#include <iostream>
#include <ctime>

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

    //Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    //Private functions
    void initVariables();
    void initWindow();
    void initEnemies();


    public:
        //Constructors / Destructors
        Game();
        virtual ~Game();

        //Accessors
        const bool running() const;

        //Function
        void spawnEnemy();
        void pollEvents();

        void updateMousePos();
        void updateEnemies();
        void update();

        void renderEnemies();
        void render();
};