#include "Engine.h"
#include "EngineStuffs/WorldManager.h"
#include "EngineStuffs/Audio/AudioSystem.h"
#include "EngineStuffs/UI/UISystem.h"
#include "Utils/ResourceLoader.h"
#include "Utils/Timing.h"

#define WORLD WorldManager::getInstance()->GetCurrentWorld()

//Creates window, Initializes low level systems and loads scene.
Engine::Engine()
{
    engine = EngineState();
    engine.quit = false;

    engine.physics = new PhysicsSystem();

    engine.system = System::Create();
    engine.system->Init();

    EventSystem* ev = EventSystem::createInstance();
    ev->init();
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
    
    delete engine.gameInputs;

    delete engine.physics;

    engine.graphics->ShutDown();
    delete engine.graphics;
    engine.graphics = nullptr;

    AudioSystem::destroyInstance();

    engine.system->Shutdown();
    delete engine.system;
}

void Engine::SceneLoad() const
{
    SceneLoader::LoadData(WorldManager::getInstance()->GetCurrentWorldName());
    WORLD->RunTransformHierarchy();
    ActiveTracker<RigidBody*> rb = WORLD->getRBsRaw();
    unsigned int numRB = WORLD->getRBActive();
    engine.physics->InitRigidBodies(rb, numRB);
}

bool Engine::InitAudio()
{
    AudioSystem* as = AudioSystem::createInstance();
    return as->Init();
}



//Game loop
void Engine::Run()
{
    if (WORLD == nullptr)
    {
        std::cout << "A world must exist to start LILLIS" << std::endl;
        return;
    }
    engine.isRunning = true;
    //Timing::SetTime();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    while (!engine.quit)
    {
        if (WorldManager::getInstance()->SetWorld())
        {
            SceneLoad();
        }

        Timing::Tick();

        while (Timing::frameLag >= Timing::fixedUpdateTime)
        {
            frameStep();
            Timing::frameLag -= Timing::fixedUpdateTime;
        }

        renderStep();

        engine.quit = engine.graphics->isWindowOpen();
    }
#endif

}


//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{
    engine.gameInputs->UpdateControllers();

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

    ActiveTracker<RectangleCollider*> col = WORLD->getCollidersRaw();

    unsigned int lastCol = WORLD->getColActive();
    for (int i = 0; i < lastCol; i++)
    {
        for (int j = i + 1; j < lastCol; j++)
        {
            if (col[j]->GetActive() && col[i]->GetActive())
            {
                col[i]->CheckCollision(*col[j]);
            }
        }
    }

    //TODO: Add physics precision into engine settings.

    ActiveTracker<RigidBody*> rb = WORLD->getRBsRaw();
    unsigned int numRB = WORLD->getRBActive();
    //TODO: make a double please!
    engine.physics->PhysicsStep((float)Timing::fixedUpdateTime, rb, numRB,3);

    //First run all possible changes, then run hierarchy.
    WORLD->RunTransformHierarchy();

    //WORLD->doRenderOrder(toUpdate);

    //TODO: Find a place to put this.
    //WORLD->compactObjects(.GetNumActive());
    WORLD->compactColliders(col.GetNumInactive());
    WORLD->compactAnimators(anims.GetNumInactive());
    WORLD->compactRigidBodies(rb.GetNumInactive());


#ifdef _DEBUG
    if (engine.gameInputs->getIsKeyDown(LILLIS::ESC))
    {
        engine.graphics->closeWindow();
    }
#endif
}

void Engine::renderStep()
{
    engine.graphics->PreDraw();

    ActiveTracker<Sprite*> sprites = WORLD->getSpritesRaw();
    unsigned int lastSpr = WORLD->getSprActive();

    engine.graphics->RenderCall(sprites, lastSpr, WORLD->getTileMaps());

    UISystem::getInstance()->RenderUI();

    engine.graphics->PostDraw();

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



std::string GetFileName(const char* path)
{
    stringstream ss = stringstream(path);
    std::string currentString;
    while (!ss.eof())
    {
        currentString.clear();
        std::getline(ss, currentString, '/');
    }
    return currentString;
}

//Call this once for each type of asset you'd like to import.
void Engine::InjectAssets(const char* filePath, AssetType resourceType)
{
    switch (resourceType)
    {
        case SPRITE:
            ResourceLoader::LoadTextureRecursive(filePath, true);
            break;
        case SHADERS:
            ResourceLoader::LoadShaderRecursive(filePath);
            break;
        case SOUNDS:
            std::cout << "Not implemented yet" << '\n';
            break;
        case DATA:
            ResourceLoader::LoadDataRecursive(filePath);
            break;
        default:
            std::cerr << "INVALID ASSET INSERTION" << '\n';
            break;
    }
}

void Engine::InjectSingleAsset(const char* filePath, AssetType resourceType)
{
    std::string FileName = GetFileName(filePath);

    switch (resourceType)
    {
        case SPRITE:
            ResourceLoader::LoadTexture(filePath, true);
            break;
        case SHADERS:
            std::cerr << "SINGLE SHADER INSERTION PROHIBITED" << '\n';
            break;
        case SOUNDS:
            std::cout << "Not implemented yet" << '\n';
            break;
        case DATA:
            ResourceLoader::LoadData(filePath);
            break;
        default:
            std::cerr << "INVALID ASSET INSERTION" << '\n';
            break;
    }
}

void Engine::LoadImportData(const char *filePath)
{
    ResourceLoader::LoadProjectInfo(filePath);
}


void Engine::Init(RenderSettings render_settings, std::string gameName, std::vector<LILLIS::KeyCode> keys)
{

    engine.graphics = DBG_NEW GraphicsSystem(render_settings, gameName);
    if (!engine.graphics->Init())
    {
        exit(1);
    }

    engine.gameInputs = DBG_NEW InputSystem(keys);
    engine.gameInputs->setupKeyInputs(engine.graphics->GetWin());

    Timing::Init();

    WorldManager::createInstance();

    UISystem* uiSys = UISystem::createInstance(render_settings);
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