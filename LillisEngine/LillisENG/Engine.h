#pragma once

#include "EngineStuffs/System.h"
#include "EngineStuffs/SceneLoader.h"
#include "EngineStuffs/GameObjectManager.h"
#include "EngineStuffs/StackAllocator.h"
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
    GPR460::System* system;
    Uint32 frameStart;
    bool quit;
    int frame;
};


class DLLUSAGE Engine
{
public:
    Engine() {};
    ~Engine() {};

    void Run();
private:
    void runMainLoop(EngineState* engine);
    void frameStep(void* arg);
    Uint32 GetTicks();
};

