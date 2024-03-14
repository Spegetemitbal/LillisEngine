#include "pch.h"
#pragma once

//Engine.h + cpp = The central game and all of its attachments.

#include "System/System.h"
#include "Utils/SceneLoader.h"
#include "EngineStuffs/GameObject.h"
#include "Utils/StackAllocator.h"

#define WORLD GameObjectManager::world

#ifdef LILLISENG
#define DLLUSAGE _declspec(dllexport)
#else
#define DLLUSAGE _declspec(dllimport)
#endif // GPR460ENG


struct EngineState
{
    GraphicsSystem* graphics;
    LILLIS::System* system;
    double frameStart;
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
    EngineState engine;

    Engine();
    ~Engine();

    //StackAllocator* FrameAllocator = DBG_NEW StackAllocator();
    void frameStep();
};

