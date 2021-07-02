#include <SDL2/SDL.h>
#include <stdio.h>
#include "mulperi.h"

int main(int argc, char *argv[])
{

    Mulperi::Config config = {"mulperi engine", 512, 512, false, 60};
    // Mulperi::SDLRendererWrapper renderer;
    // Mulperi::Game myGame(config, renderer);

    // myGame.Run();

    return EXIT_SUCCESS;
}