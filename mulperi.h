#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>

using namespace std;

namespace Mulperi
{
    typedef struct Config
    {
        string title;
        int screenWidth;
        int screenHeight;
        bool fullScreen;
        int fps;
    } Config;

    class Renderer
    {
    public:
        Renderer(){};
        virtual ~Renderer(){};
        virtual void Render();
    };

    class SDLRendererWrapper : Renderer
    {
        SDL_Renderer *renderer;
        SDL_Window *window;

    public:
        SDLRendererWrapper()
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
        ~SDLRendererWrapper()
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

    class Controls
    {
    public:
        Controls() {}
        void HandleInput(bool &running)
        {
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

    class Game
    {
        Simulation sim;
        Renderer renderer;
        Controls controls;
        Config config;
        bool running;

    public:
        Game(Config gameConfig, Renderer gameRenderer) : running(true)
        // renderer(gameRenderer)
        {
            config = gameConfig;
        }
        ~Game()
        {
            SDL_Quit();
        }
        void Run()
        {
            // while (running)
            // {
            //     // sim.Update();
            //     controls.HandleInput(running);
            //     renderer.Render(); // fps delay is in here
            // }
        }
    };

}