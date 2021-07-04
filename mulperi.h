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

    class Actor
    {
    public:
        Input *input;
        struct pos
        {
            float x;
            float y;
        } pos;
        Actor() {}
        Actor(Input *i, std::string type, float x, float y) : pos({x, y})
        {
            input = i;
        }
        virtual void Update() = 0;
        virtual void Render() = 0;
    };

    class Renderer
    {
    public:
        virtual void Render(const std::unordered_map<std::string, Actor *> &actors) = 0;
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
        void Render(const std::unordered_map<std::string, Actor *> &actors) override
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderClear(renderer);

            for (const auto it : actors)
            {
                SDL_Rect rect = {(int)it.second->pos.x, (int)it.second->pos.y, 50, 50};
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000 / 60); // Pause
        }
    };

    class Simulation
    {
    public:
        void Update(const std::unordered_map<std::string, Actor *> &actors)
        {
            for (const auto it : actors)
            {
                it.second->Update();
            }
        };
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

    class ActorManager
    {
    public:
        std::unordered_map<std::string, Actor *> actors;
        void CreateActor(std::string name, Actor *actor)
        {
            actors[name] = actor;
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
        Config config;
        bool running;

    public:
        ActorManager actorManager;
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
                sim.Update(actorManager.actors);
                renderer->Render(actorManager.actors); // fps delay is in here
                input->HandleInput(running);
            }
        }
        void CreateActor(std::string name, Actor *actor)
        {
            actorManager.CreateActor(name, actor);
        }
        void DeleteActor(std::string name)
        {
            actorManager.DeleteActor(name);
        }
    };

}