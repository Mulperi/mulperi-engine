#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>

namespace Mulperi
{
    typedef struct Config
    {
        std::string title;
        int screenWidth;
        int screenHeight;
        bool fullScreen;
        int fps;
    } Config;

    class Renderer
    {
    public:
        virtual void Render() = 0;
    };

    class RendererWrapperSDL : public Renderer
    {
        SDL_Renderer *renderer;
        SDL_Window *window;

    public:
        RendererWrapperSDL()
        {
            SDL_Init(SDL_INIT_VIDEO);
            window = SDL_CreateWindow("window name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
            if (window == NULL)
            {
                printf("Could not create window: %s\n", SDL_GetError());
            }
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL)
            {
                printf("Could not create renderer: %s\n", SDL_GetError());
            }
        }
        ~RendererWrapperSDL()
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
        void Render() override
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderClear(renderer);

            SDL_Rect rect = {100, 100, 100, 100};
            SDL_RenderDrawRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);

            SDL_RenderPresent(renderer);
            SDL_Delay(300); // Pause
        }
    };

    class Simulation
    {
    public:
        void Update(){};
    };

    class Input
    {
    public:
        struct keyboard
        {
            bool up;
            bool down;
            bool left;
            bool right;
            // jne.
        } keyboard;
        virtual void HandleInput(bool &running) = 0;
    };

    class InputWrapperSDL : public Input
    {
        const Uint8 *keyState;

    public:
        InputWrapperSDL() : keyState(nullptr){};
        void HandleInput(bool &running) override
        {
            keyState = SDL_GetKeyboardState(NULL);
            if (keyState[SDL_SCANCODE_UP])
            {
                keyboard.up = true;
            }
            else
            {
                keyboard.up = false;
            }

            if (keyState[SDL_SCANCODE_DOWN])
            {
                keyboard.down = true;
            }
            else
            {
                keyboard.down = false;
            }

            if (keyState[SDL_SCANCODE_LEFT])
            {
                keyboard.left = true;
            }
            else
            {
                keyboard.left = false;
            }

            if (keyState[SDL_SCANCODE_RIGHT])
            {
                keyboard.right = true;
            }
            else
            {
                keyboard.right = false;
            }

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_WINDOWEVENT_CLOSE:
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    };

    class Actor
    {
    public:
        virtual void Update() = 0;
        virtual void Render() = 0;
    };

    class ActorManager
    {
        std::unordered_map<std::string, Actor *> actors;

    public:
        void CreateActor(std::string name, Actor *actor)
        {
            actors.at(name) = actor;
        }
        void DeleteActor(std::string name)
        {
            actors.erase(name);
        }
    };

    class Game
    {
        Simulation sim;
        Renderer *renderer;
        Input *input;
        ActorManager actorManager;
        Config config;
        bool running;

    public:
        Game(Config gameConfig,
             Renderer *gameRenderer,
             Input *gameInput) : config(gameConfig),
                                 renderer(gameRenderer),
                                 input(gameInput), running(true)
        {
        }
        ~Game()
        {
            SDL_Quit();
        }
        void Run()
        {
            while (running)
            {
                // sim.Update();
                input->HandleInput(running);
                renderer->Render(); // fps delay is in here
            }
        }
        void CreateActor(std::string name, Actor *actor)
        {
            actorManager.CreateActor(name, actor);
        }
        void DeleteActor(std::string name)
        {
            actorManager.CreateActor(name);
        }
    };

}