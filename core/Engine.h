#pragma once

//Engine.h + cpp = The central game and all of its attachments.

#include "System/System.h"
#include "Utils/SceneLoader.h"
#include "EngineStuffs/GameObject.h"
#include "Utils/StackAllocator.h"
#include "Utils/InputSystem.h"
#include "EngineStuffs/Graphics/RenderSettings.h"
#include "System/System_Common.h"
#include "Utils/ResourceManager.h"


struct EngineState
{
    GraphicsSystem* graphics = nullptr;
    PhysicsSystem* physics = nullptr;
    InputSystem* gameInputs = nullptr;
    LILLIS::System* system = nullptr;
    bool quit = false;
    bool isRunning = false;
};

class Engine
{
public:
    static Engine* Game;

    static Engine* GetGameInstance();
    static Engine* CreateGameInstance();
    static void DestroyGameInstance();

    //TODO Split this
    void Init(RenderSettings render_settings, std::string gameName, std::vector<LILLIS::KeyCode>);

    //Do after loading import data.
    bool InitAudio();

    //TODO: Make better access to graphics and physics system data, this won't cut it. Try dependency injection?
    const GraphicsSystem& GetGraphicsSystem() const
    {
        return *engine.graphics;
    }

    void InjectAssets(const char* filePath, AssetType resourceType);
    void InjectSingleAsset(const char* filePath, AssetType resourceType);
    void LoadImportData(const char* filePath);

    void Run();
private:
    EngineState engine;

    Engine();
    ~Engine();

    //StackAllocator* FrameAllocator = DBG_NEW StackAllocator();
    void SceneLoad() const;
    void frameStep();
    void renderStep();
    void updateScripts();
};

