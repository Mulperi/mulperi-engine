#include <SDL2/SDL.h>
#include <stdio.h>
#include "mulperi.h"
#include <string>
// Windows compile: g++ main.cpp -o main.exe -I include -L . -lmingw32 -lSDL2 -lSDL2main
// Linux compile: g++ main.cpp -o main I /usr/include -lSDL2

// Actor need to call base class constructor with pointer to input manager.
class Player : public Mulperi::Actor
{
public:
    Player(Mulperi::Input *i, std::string type, float x, float y) : Mulperi::Actor(i, type, x, y){};

    void Update() override
    {
        if (inputManager->keyboard.up)
        {
            pos.y--;
        }
        if (inputManager->keyboard.down)
        {
            pos.y++;
        }
        if (inputManager->keyboard.left)
        {
            pos.x--;
        }
        if (inputManager->keyboard.right)
        {
            pos.x++;
        }
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

    Player player1(&input, "rect", 100, 100); // give pointer to input manager

    myGame.CreateActor("player", &player1);

    myGame.Run();

    return EXIT_SUCCESS;
}
