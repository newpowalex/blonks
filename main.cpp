////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "Game.h"

int main()
{
    //Init game engine
    Game game;
    //std::cout << "Started";

    //Game loop
    while (game.running())
    {
        //Update
        game.update();
        //std::cout << "updating";

        //Render
        game.render();
    }

    //End of application
    return 0;
}
