#include <SDL2/SDL.h>
#include <stdio.h>
#include "mulperi.h"
#include <string>
// Windows compile: g++ main.cpp -o main.exe -I include -L . -lmingw32 -lSDL2 -lSDL2main
// Linux compile: g++ main.cpp -o main I /usr/include -lSDL2

class Player : public Mulperi::Actor
{
    Player();
    void Update() override
    {
    }
    void Render() override
    {
    }
};

int main(int argc, char *argv[])
{

    Mulperi::Config config = {"mulperi engine", 512, 512, false, 60};
    Mulperi::RendererWrapperSDL renderer;
    Mulperi::InputWrapperSDL input;
    Mulperi::Game myGame(config, &renderer, &input);

    Player player1();

    myGame.CreateActor("player", player1);

    myGame.Run();

    return EXIT_SUCCESS;
}