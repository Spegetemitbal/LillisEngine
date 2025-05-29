#include "Engine.h"
#include "EngineStuffs/WorldManager.h"
#include "EngineStuffs/Audio/AudioSystem.h"
#include "EngineStuffs/UI/UISystem.h"
//#include "Utils/ResourceLoader.h"
#include "EngineStuffs/Graphics/ProcGen.h"
#include "Utils/Timing.h"

#define WORLD WorldManager::getInstance()->GetCurrentWorld()

//Creates window, Initializes low level systems and loads scene.
Engine::Engine()
{
    timeToQuit = false;
    system = System::Create();
    system->Init();
    Timing::Init();

    WorldManager::createInstance();
}

//Clears everything
Engine::~Engine()
{
    //WORLD->DelWorld();
    WorldManager::destroyInstance();
    //delete FrameAllocator

    EventSystem* ev = EventSystem::getInstance();
    ev->cleanup();
    EventSystem::delInstance();

    PhysicsSystem::destroyInstance();

    GraphicsSystem::delInstance();

    AudioSystem::destroyInstance();

    system->Shutdown();
    delete system;
}



//Game loop
void Engine::Run()
{
    if (WORLD == nullptr)
    {
        std::cout << "A world must exist to start LILLIS" << std::endl;
        return;
    }

    if (!GraphicsSystem::getInstance()->GetIsInitted())
    {
        std::cout << "Graphics System must be initialized to start LILLIS" << std::endl;
        return;
    }

    isRunning = true;
    //Timing::SetTime();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    while (!timeToQuit)
    {
        WorldManager* wm = WorldManager::getInstance();
        wm->SetWorld();

        Timing::Tick();

        while (Timing::frameLag >= Timing::fixedUpdateTime)
        {
            frameStep();
            Timing::frameLag -= Timing::fixedUpdateTime;
        }

        if (!wm->IsChangingWorldsNextFrame())
        {
            renderStep();
        }

        timeToQuit = GraphicsSystem::getInstance()->isWindowOpen();
    }
#endif

}


//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{
    InputSystem::UpdateControllers();
    //TODO: Move this elsewhere.
    ProcGen::getInstance()->ClearDebugObjects();

    updateScripts();

    ActiveTracker<Animator*> anims = WORLD->getAnimatorsRaw();
    unsigned int lastAnim = WORLD->getAnimActive();
    for (int i = 0; i < lastAnim; i++)
    {
        if (anims[i]->GetActive())
        {
            anims[i]->Update(Timing::fixedUpdateTime);
        }
    }

    ActiveTracker<RigidBody*> rb = WORLD->getRBsRaw();
    unsigned int numRB = WORLD->getRBActive();
    PhysicsSystem* phys = PhysicsSystem::getInstance();
    phys->PhysicsStep(Timing::fixedUpdateTime, rb, numRB);

    //First run all possible changes, then run hierarchy.
    WORLD->RunTransformHierarchy();
    phys->ChildTriggerUpdate(rb, numRB);

    //WORLD->doRenderOrder(toUpdate);

    //TODO: Find a place to put this.
    //WORLD->compactObjects(.GetNumActive());
    WORLD->compactAnimators(anims.GetNumInactive());
    WORLD->compactRigidBodies(rb.GetNumInactive());


#ifdef _DEBUG
    if (InputSystem::getIsKeyDown(LILLIS::ESC))
    {
        GraphicsSystem::getInstance()->closeWindow();
    }
#endif
}

void Engine::renderStep()
{
    GraphicsSystem* graphics = GraphicsSystem::getInstance();

    graphics->PreDraw();

    ActiveTracker<Sprite*> sprites = WORLD->getSpritesRaw();
    unsigned int lastSpr = WORLD->getSprActive();

    graphics->RenderCall(sprites, lastSpr, WORLD->getTileMaps());

    UISystem::getInstance()->RenderUI();

    graphics->PostDraw();

    //End of Frame Garbage collection
    WORLD->compactSprites(sprites.GetNumInactive());
}

void Engine::updateScripts()
{
    ActiveTracker<Behavior*> behvs = WORLD->getBehaviorsRaw();
    int bevSize = behvs.size();
    for (int i = 0; i < bevSize; i++)
    {
        if (behvs[i]->GetActive())
        {
            behvs[i]->Update((float)Timing::fixedUpdateTime);
        }
    }
    WORLD->compactBehaviors(behvs.GetNumInactive());
}

//Singleton getter
Engine* Engine::GetGameInstance()
{
    if (Game != nullptr)
    {
        return Game;
    }
    else
    {
        return nullptr;
    }
}

//Singleton create (No lazy init is used)
Engine* Engine::CreateGameInstance()
{
    if (Game == nullptr)
    {
        Game = DBG_NEW Engine();
    }
    return Game;
}

//Singleton destroy
void Engine::DestroyGameInstance()
{
    delete Game;
    Game = nullptr;
}

Engine* Engine::Game = nullptr;