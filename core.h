// old c-like version of sdl helper functions, just for reference

#ifndef MULPERICORE_H
#define MULPERICORE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

enum SPRITE_SOURCE
{
    SPRITE_SRC_IMAGE,
    SPRITE_SRC_TEXT
};

typedef struct
{
    int r, g, b, a;
} Color;

typedef struct
{
    float x, y, max;
} Velocity;

typedef struct
{
    int x, y, active, type;
    Color color;
    Velocity vel;
} ParticleSimple;

typedef struct Body
{
    SDL_Rect rect;
    float mass;
    float radius;
    float angle;
} Body;

typedef struct MulperiSprite
{
    int texture_id;
    Body body;
    float zoom;
} MulperiSprite;

typedef struct MulperiConfig
{
    int windowWidth;
    int windowHeight;
    int logicalWidth;
    int logicalHeight;
    int fps;
    int debug;
    std::string title;
} MulperiConfig;

typedef struct Mouse
{
    SDL_Point pos;
} Mouse;

typedef struct RenderSettings
{
    SDL_Color clear_color;
    int clear_enabled;
} RenderSettings;

typedef struct Mulperi
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    RenderSettings render_settings;
    SDL_Event event;
    MulperiConfig config;
    Mouse mouse;
    int key_up;
    const Uint8 *key_state;
    int start_time;
    void (*update)(); // Custom function pointer
    void (*render)(); // Custom fnction pointer
    int running;
} Mulperi;

Mulperi engine;

/*
Engine core functions
*/
void mulperi_init(int windowWidth, int windowHeight, int logicalWidth, int logicalHeight, int fps, int debug, std::string title)
{
    srand(time(NULL));
    engine.start_time = time(NULL);
    engine.config.debug = debug;
    engine.config.windowWidth = windowWidth;
    engine.config.windowHeight = windowHeight;
    engine.config.logicalWidth = logicalWidth;
    engine.config.logicalHeight = logicalHeight;
    engine.config.fps = fps;
    engine.config.debug = debug;
    engine.config.title = title;

    SDL_Init(SDL_INIT_EVERYTHING);
    engine.window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(engine.renderer, logicalWidth, logicalHeight);

    engine.render_settings.clear_enabled = 1;
    engine.running = 1;
}

void mulperi_quit()
{
    // Destroy audio stuff.
    // Mix_CloseAudio();
    // Mix_Quit();

    // Destroy font stuff.
    // CloseFonts();
    // if (TTF_WasInit())
    //     TTF_Quit();

    // Destroy textures.
    // DestroyTextures();

    // Destory window and renderer.
    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);
    SDL_Quit();
    printf("Succesfully closed libraries.\n");
}

int mulperi_process_events()
{
    while (SDL_PollEvent(&engine.event))
    {
        switch (engine.event.type)
        {
        case SDL_QUIT:
            return 0;
            break;
        case SDL_MOUSEMOTION:
            if (1)
            {
                engine.mouse.pos.x = (int)engine.event.motion.x;
                engine.mouse.pos.y = (int)engine.event.motion.y;
            }
            break;
        case SDL_KEYUP:
            engine.key_up = engine.event.key.keysym.sym;
            switch (engine.event.key.keysym.sym)
            {
            case SDLK_F11:
                if (engine.config.debug)
                    printf("Full screen toggle.\n");
                if (SDL_GetWindowFlags(engine.window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
                {
                    SDL_SetWindowFullscreen(engine.window, 0);
                    SDL_SetWindowSize(engine.window, engine.config.windowWidth, engine.config.windowHeight);
                }
                else
                {
                    Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(engine.window, flags);
                }
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }
    }
    return 1;
}

void mulperi_update()
{
    Uint32 frame_start = SDL_GetTicks();
    engine.key_state = SDL_GetKeyboardState(NULL);

    engine.update();

    Uint32 frame_end = SDL_GetTicks();
    SDL_Delay(1000 / engine.config.fps - (frame_end - frame_start));
}

void mulperi_update_set(void (*custom_update_func)())
{
    engine.update = custom_update_func;
}

void mulperi_render()
{
    if (engine.render_settings.clear_enabled)
    {
        SDL_SetRenderDrawColor(engine.renderer, engine.render_settings.clear_color.r,
                               engine.render_settings.clear_color.g,
                               engine.render_settings.clear_color.b,
                               engine.render_settings.clear_color.a);
        SDL_RenderClear(engine.renderer);
    }

    engine.render();

    SDL_RenderPresent(engine.renderer);
}

void mulperi_render_set(void (*custom_render_func)())
{
    engine.render = custom_render_func;
}

void mulperi_render_clear_color_set(int r, int g, int b, int a)
{
    engine.render_settings.clear_color.r = r;
    engine.render_settings.clear_color.g = g;
    engine.render_settings.clear_color.b = b;
    engine.render_settings.clear_color.a = a;
}

void mulperi_render_clear_enable()
{
    engine.render_settings.clear_enabled = 1;
}

void mulperi_render_clear_disable()
{
    engine.render_settings.clear_enabled = 0;
}

void mulperi_start()
{
    while (engine.running)
    {
        engine.running = mulperi_process_events();
        mulperi_update();
        mulperi_render();
    }
    mulperi_quit();
}

void mulperi_stop()
{
    engine.running = 0;
}

/*
Events
*/

int mulperi_key_up(int key)
{
    if (engine.key_up == key)
    {
        return 1;
    }
    return 0;
}

int mulperi_key_down(int key)
{
    return engine.key_state[key];
}

int mulperi_mouse_down(int button)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button))
    {
        return true;
    }
    return false;
}

int mulperi_mouse_x()
{
    return engine.mouse.pos.x;
}

int mulperi_mouse_y()
{
    return engine.mouse.pos.y;
}

/*
Utility functions.
*/
int mulperi_uptime()
{
    return time(NULL) - engine.start_time;
}

int mulperi_random_number(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

int mulperi_inside_area(int x, int y, int w, int h, int checkWidth, int checkHeight)
{
    if (
        x > 0 &&
        x < (checkWidth + w) &&
        y > 0 &&
        y < (checkHeight + h))
    {
        return 1;
    }
    return 0;
}

int mulperi_collision_rect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    if (x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2)
    {
        return 1;
    }
    return 0;
}

/*
Draw functions
*/
void mulperi_draw_point(int x, int y, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(engine.renderer, r, g, b, a);
    SDL_RenderDrawPoint(engine.renderer, x, y);
}

#endif