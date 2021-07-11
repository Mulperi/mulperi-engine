#pragma once
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <time.h>
#include "include/box2d/box2d.h"
namespace Mulperi
{
    float SCALE = 30.f;

    typedef struct World
    {
        // b2Vec2 gravity;
        b2World world;
    } World;

    enum BODY_TYPE
    {
        BODY_STATIC,
        BODY_DYNAMIC,
        SKIP_PHYSICS
    };

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

        b2BodyDef bodyDef;
        b2Body *body;
        b2PolygonShape shape;
        b2FixtureDef fixture;

        Actor() {}
        Actor(Input *i, std::string type, float x, float y, BODY_TYPE bodyType, b2World *world) : pos()
        {
            input = i;
            // pos.x = x;
            // pos.y = y;
            body = world->CreateBody(&bodyDef);
            bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
            if (bodyType == BODY_DYNAMIC)
            {
                bodyDef.type = b2_dynamicBody;
            }

            shape.SetAsBox((50.f / 2) / SCALE, (50.f / 2) / SCALE);
            fixture.density = 1.f;
            fixture.friction = 0.7f;
            fixture.shape = &shape;
            body->CreateFixture(&fixture);
        }
        virtual void Update() = 0;
        virtual void Render() = 0;
    };

    class Renderer
    {
    public:
        virtual void Render(const std::unordered_map<std::string, Actor *> &actors, b2World *world) = 0;
    };

    class Simulation
    {

    public:
        b2Vec2 gravity;
        b2World world;
        Simulation() : gravity(b2Vec2(0, -10)), world(b2World(gravity))
        {
        }
        void Update(const std::unordered_map<std::string, Actor *> &actors)
        {
            world.Step(1 / 60.f, 8, 3);
            // Render actors.
            for (std::pair<std::string, Actor *> it : actors)
            {
                it.second->Update();
            }
        };
    };

    class Scene
    {
        Input *input;

    public:
        std::unordered_map<std::string, Actor *> actors;
        Scene(Input *i) : input(i) {}
        // virtual ~Scene() = 0;
        virtual void Update() = 0;
        void AttachActor(std::string actorName, Actor *actor)
        {
            actors[actorName] = actor;
        }
        void DetachActor(std::string actorName, Actor *actor)
        {
            actors.erase(actorName);
        }
    };

    class SceneManager
    {
    private:
        // std::unordered_map<std::string, std::unordered_map<std::string, Actor *>> scenes;
        std::unordered_map<std::string, Scene *> scenes;

    public:
        std::string currentSceneName;
        // void AttachActorToScene(std::string actorName, Actor *actor, std::string sceneName)
        // {
        //     scenes[sceneName][actorName] = actor;
        // }
        // void DetachActorFromScene(std::string actorName, Actor *actor, std::string sceneName)
        // {
        //     scenes[sceneName].erase(actorName);
        // }
        void SetCurrentSceneName(std::string name)
        {
            currentSceneName = name;
        }
        std::unordered_map<std::string, Actor *> GetCurrentSceneActors()
        {
            return scenes[currentSceneName]->actors;
        }
        void AttachScene(std::string sceneName, Scene *scene)
        {
            scenes[sceneName] = scene;
        }
        void DetachScene(std::string sceneName)
        {
            scenes.erase(sceneName);
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
            running = true;
        }
        b2World *GetWorld()
        {
            return &sim.world;
        }
        void Run()
        {
            while (running)
            {
                uptime = time(NULL) - starttime;
                sim.Update(sceneManager.GetCurrentSceneActors());
                renderer->Render(sceneManager.GetCurrentSceneActors(), GetWorld()); // fps delay is in here
                input->HandleInput(running);
                Update();
            }
        }
    };
}