#pragma once
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <time.h>

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

    class SceneManager
    {
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, Actor *>> scenes;

    public:
        std::string currentSceneName;
        void AttachActorToScene(std::string actorName, Actor *actor, std::string sceneName)
        {
            scenes[sceneName][actorName] = actor;
        }
        void DetachActorFromScene(std::string actorName, Actor *actor, std::string sceneName)
        {
            scenes[sceneName].erase(actorName);
        }
        void SetCurrentSceneName(std::string name)
        {
            currentSceneName = name;
        }
        std::unordered_map<std::string, Actor *> GetCurrentSceneActors()
        {
            return scenes[currentSceneName];
        }
    };

    class Game
    {
    private:
        Simulation sim;
        Renderer *renderer;
        Input *input;
        Config config;
        bool running;
        int starttime;
        // void (*userUpdate)();

    public:
        int uptime;
        SceneManager sceneManager;
        virtual void Update() = 0;
        virtual ~Game() {}
        Game(Config gameConfig,
             Renderer *gameRenderer,
             Input *gameInput) : config(gameConfig),
                                 renderer(gameRenderer),
                                 input(gameInput)
        {
            starttime = time(NULL);
        }

        void Run()
        {
            while (running)
            {
                uptime = time(NULL) - starttime;
                sim.Update(sceneManager.GetCurrentSceneActors());
                renderer->Render(sceneManager.GetCurrentSceneActors()); // fps delay is in here
                input->HandleInput(running);
                Update();
            }
        }
    };

}