#pragma once

#include "System/System.h"
#include "Utils/SceneLoader.h"
#include "EngineStuffs/GameObject.h"
#include "Utils/StackAllocator.h"
#include "Utils//InputSystem.h"
#include <iostream>
#include <vector>

#define WORLD GameObjectManager::world
StackAllocator* FrameAllocator = DBG_NEW StackAllocator();

#ifdef LILLISENG
#define DLLUSAGE _declspec(dllexport)
#else
#define DLLUSAGE _declspec(dllimport)
#endif // GPR460ENG


struct EngineState
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    GPR460::System* system;
    Uint32 frameStart;
    bool quit;
    int frame;

    bool isEditor;
};


class DLLUSAGE Engine
{
public:
    static Engine* Game;

    static Engine* GetGameInstance();
    static Engine* CreateGameInstance();
    static void DestroyGameInstance();

    void LoadLevel(std::string Data);
    void Run();
private:
    EngineState* currentState;

    Engine();
    ~Engine();

    void frameStep(void* arg);
    Uint32 GetTicks();
};

