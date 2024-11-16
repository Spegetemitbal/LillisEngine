#include "pch.h"
#pragma once

//Engine.h + cpp = The central game and all of its attachments.

#include "System/System.h"
#include "Utils/SceneLoader.h"
#include "EngineStuffs/GameObject.h"
#include "Utils/StackAllocator.h"
#include "Utils/InputSystem.h"
#include "EngineStuffs/GameObjectManager.h"

#define WORLD GameObjectManager::world

#ifdef LILLISENG
#define DLLUSAGE __declspec(dllexport)
#else
#define DLLUSAGE __declspec(dllimport)
#endif


struct EngineState
{
    GraphicsSystem* graphics;
    InputSystem* gameInputs;
    LILLIS::System* system;
    PhysicsSystem* phys;
    double frameStart;
    bool quit;
    int frame;

    bool isEditor;
};

namespace fs = std::filesystem;

enum DLLUSAGE AssetType
{
    SINGLEIMAGES = 0,
    SPRITESHEET,
    SHADERS,
    SOUNDS

};

class DLLUSAGE Engine
{
public:
    static Engine* Game;

    static Engine* GetGameInstance();
    static Engine* CreateGameInstance();
    static void DestroyGameInstance();

    void InjectAssets(const char* filePath, AssetType resourceType);
    void InjectSingleAsset(const char* filePath, AssetType resourceType);

    void LoadLevel(std::string Data);
    void Run();
private:
    EngineState engine;

    Engine();
    ~Engine();

    //StackAllocator* FrameAllocator = DBG_NEW StackAllocator();
    void frameStep();

    //Everything past this point is game specific and to be wiped upon abstraction.
    void restartGame();

    GameObject* p1;
    GameObject* p2;
};

