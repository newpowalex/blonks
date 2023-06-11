#pragma once

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

    //Game objects
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
        void pollEvents();
        void update();
        void render();
};