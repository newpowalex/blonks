////////////////////////////////////////////////////////////
// Headers /////////////////////////////////////////////////
////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "Game.h"

int main()
{
    //Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Init game engine
    Game game;

    //Game loop
    while (game.running())
    {
        switch (game.getGameState())
        {
        case Game::GameState::StartScreen:
            game.updateStartScreen();
            game.renderStartScreen();
            break;
        case Game::GameState::InGame:
            game.update();
            game.render();
            break;
        }
    }

    //End of application
    return 0;
}
