#include <SDL2/SDL.h>
#include <stdio.h>
#include "mulperi.h"

// Windows compile: g++ main.cpp -o main.exe -I include -L . -lmingw32 -lSDL2 -lSDL2main
// Linux compile: g++ main.cpp -o main I /usr/include -lSDL2

int main(int argc, char *argv[])
{

    Mulperi::Config config = {"mulperi engine", 512, 512, false, 60};
    Mulperi::SDLRendererWrapper renderer;
    Mulperi::Game myGame(config, &renderer);

    myGame.Run();

    return EXIT_SUCCESS;
}