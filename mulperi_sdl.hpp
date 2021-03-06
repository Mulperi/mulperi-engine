#include "include/SDL2/SDL.h"
#include "mulperi.hpp"
#include "include/box2d/box2d.h"

namespace Mulperi
{
    class RendererWrapperSDL : public Mulperi::Renderer
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
            SDL_Quit();
        }
        void Render(const std::unordered_map<std::string, Mulperi::Actor *> &actors, b2World *world) override
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderClear(renderer);

            for (b2Body *BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
            {
                if (BodyIterator->GetType() == b2_dynamicBody)
                {
                    SDL_Rect rect = {static_cast<int>(Mulperi::SCALE * BodyIterator->GetPosition().x), static_cast<int>(Mulperi::SCALE * BodyIterator->GetPosition().y), 100, 100};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    // sf::RectangleShape Sprite(sf::Vector2f(50.f, 50.f));
                    // Sprite.setFillColor(sf::Color(100, 100, 255));
                    // Sprite.setTexture(BoxTexture);
                    // Sprite.setOrigin(16.f, 16.f);
                    // Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                    // Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                    // window.draw(Sprite);
                    // ++BodyCount;
                }
            }

            // for (const auto it : actors)
            // {
            //     SDL_Rect rect = {(int)it.second->pos.x, (int)it.second->pos.y, 50, 50};
            //     SDL_RenderDrawRect(renderer, &rect);
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //     SDL_RenderFillRect(renderer, &rect);
            // }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000 / 60); // Pause
        }
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
};
