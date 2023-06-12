////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "Game.h"

int main()
{
    // Redirect output to a file
    std::ofstream outputFile("output.log");
    std::streambuf* coutBuffer = std::cout.rdbuf(outputFile.rdbuf());
    std::streambuf* cerrBuffer = std::cerr.rdbuf(outputFile.rdbuf());

    //Init game engine
    Game game;
    std::cout << "Started";

    //Game loop
    while (game.running())
    {
        //Update
        game.update();
        std::cout << "updating";

        //Render
        game.render();
    }

    //End of application
    return 0;
}
