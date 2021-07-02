#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",       // window title
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        640,                    // width, in pixels
        480,                    // height, in pixels
        SDL_WINDOW_OPENGL       // flags - see below
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Check that the window was successfully created
    if (window == NULL)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // The window is open: could enter program loop here (see SDL_PollEvent())

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {

            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);

            switch (event.type)
            {
            case SDL_WINDOWEVENT_CLOSE:
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;

                SDL_Delay(300); // Pause execution for 300 milliseconds, for example
            }
        }
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return EXIT_SUCCESS;
}