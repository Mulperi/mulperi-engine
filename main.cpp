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
        if (input->keyboard.up)
        {
            pos.y--;
        }
        if (input->keyboard.down)
        {
            pos.y++;
        }
        if (input->keyboard.left)
        {
            pos.x--;
        }
        if (input->keyboard.right)
        {
            pos.x++;
        }
    }
    void Render() override
    {
    }
};

// class Peli : public Mulperi::Game
// {
// public:
//     Peli(Mulperi::Config gameConfig,
//          Mulperi::Renderer *gameRenderer,
//          Mulperi::Input *gameInput) : Mulperi::Game(gameConfig, gameRenderer, gameInput)
//     {
//     }

//     void UserUpdate() override
//     {
//         std::cout << "user update" << std::endl;
//     }
// };

void customUpdate()
{
    std::cout << "custom update" << std::endl;
}

int main(int argc, char *argv[])
{

    Mulperi::Config config = {"mulperi engine", 512, 512, false, 60};
    Mulperi::RendererWrapperSDL renderer;
    Mulperi::InputWrapperSDL input;
    Mulperi::Game myGame(config, &renderer, &input, &customUpdate);

    Player player1(&input, "rect", 100, 100); // give pointer to input manager

    myGame.CreateActor("player", &player1);
    // myGame.CreateScene("level1");
    // myGame.AddActorToScene("player", "level1");

    myGame.Run();

    return EXIT_SUCCESS;
}
