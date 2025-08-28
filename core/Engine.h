#pragma once

//Engine.h + cpp = The central game and all of its attachments.

#include "System/System.h"
#include "Utils/SceneLoader.h"
#include "EngineStuffs/GameObject.h"
#include "Utils/StackAllocator.h"
#include "Utils/InputSystem.h"
#include "EngineStuffs/Graphics/RenderSettings.h"
#include "System/System_Common.h"
//#include "Utils/ResourceManager.h"

class Engine
{
public:
    static Engine* Game;

    //TODO make this no longer a singleton- it's completely unnecessary.
    static Engine* GetGameInstance();
    static Engine* CreateGameInstance();
    static void DestroyGameInstance();

    void Run();
    void CloseApplication();
private:

    bool timeToQuit = false;
    bool isRunning = false;

    LILLIS::System* system = nullptr;

    Engine();
    ~Engine();

    void frameStep();
    void renderStep();
    void updateScripts();
    void lateUpdateScripts();
};

