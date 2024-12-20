#include "Engine.h"

#include "Utils/Timing.h"

//Creates window, Initializes low level systems and loads scene.
Engine::Engine()
{
    engine = EngineState();
    engine.quit = false;

    engine.system = LILLIS::System::Create();
    engine.system->Init();

    EventSystem* ev = EventSystem::createInstance();
    ev->init();

    const int WIDTH = 640;
    const int HEIGHT = 480;

    engine.phys = DBG_NEW PhysicsSystem();

    engine.graphics = DBG_NEW GraphicsSystem(WIDTH, HEIGHT, "Game");
    if (!engine.graphics->Init())
    {
        exit(1);
    }
    
    std::vector<LILLIS::KeyCode> importantKeys
    {
        LILLIS::W, LILLIS::S, LILLIS::A, LILLIS::D, LILLIS::ESC
    };

    engine.gameInputs = DBG_NEW InputSystem(importantKeys);
    engine.gameInputs->setupKeyInputs(engine.graphics->GetWin());

    Timing::Init();

    WORLD = DBG_NEW GameObjectManager();
}

//Clears everything
Engine::~Engine()
{
    //WORLD->DelWorld();
    delete WORLD;
    //delete FrameAllocator

    delete engine.phys;
    engine.phys = nullptr;

    EventSystem* ev = EventSystem::getInstance();
    ev->cleanup();
    EventSystem::delInstance();
    
    delete engine.gameInputs;

    engine.graphics->ShutDown();
    delete engine.graphics;
    engine.graphics = nullptr;

    engine.system->Shutdown();
    delete engine.system;
}

//Resets WORLD and loads a new scene.
void Engine::LoadLevel(std::string Data)
{
    if (WORLD == nullptr)
    {
        WORLD = DBG_NEW GameObjectManager();
    }
    else
    {
        WORLD->clearAll();
    }
    SceneLoader scl;
    scl.LoadData(Data);
}

//Game loop
void Engine::Run()
{
    //restartGame();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    while (!engine.quit)
    {
        Timing::SetTime();
        if (Timing::realTime - Timing::frameStart >= Timing::realFrameRate)
        {
            Timing::Tick();
            frameStep();
        }
        engine.quit = engine.graphics->isWindowOpen();
    }
#endif

}

//Wipe later
void Engine::restartGame()
{
    if (WORLD->numObjects > 0)
    {
        WORLD->clearAll();
    }

    LoadLevel(WORLD->CurrentLevel);
    /*
    //Game specifics
    p1 = WORLD->addObject(0, 0);
    p1->CreateCollider(40, 40, 0);
    p1->SetSprite("Player1");
    p1->CreateBehaviorGeneric("PlayerController");

    p2 = WORLD->addObject(0, 440);
    p2->SetSprite("Player2");
    p2->CreateCollider(40, 40, 1);
    p2->CreateBehaviorGeneric("PlayerController");

    LilObj<GameObject> goal = WORLD->addObject(600, 220);
    goal->SetSprite("WinFlag");
    goal->CreateCollider(40, 40, 2);

    float pos = 100;
    float ang = 20;
    for (int i = 0; i < 3; i++)
    {
        LilObj<GameObject> spinny = WORLD->addObject(300, pos);
        spinny->SetSprite("Angry");
        spinny->CreateCollider(40, 40, 3);
        spinny->CreateBehaviorGeneric("Rotator");
        LilObj<Rotator> r = spinny->GetBehavior<Rotator>("Rotator");
        r->setAngle(ang);
        r->setBaseOffset(300, pos);
        pos += 100;
        ang += 30;
    }*/

}


//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{

    ActiveTracker<Behavior*> behvs = WORLD->getBehaviorsRaw();
    for (int i = 0; i < behvs.size(); i++)
    {
        if (behvs[i]->GetActive())
        {
            behvs[i]->Update((float)Timing::deltaTime);
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

    engine.graphics->PreDraw();

    ActiveTracker<GameObject*> objects = WORLD->getObjectsRaw();
    unsigned int lastObj = WORLD->getObjActive();
    for (int i = 0; i < lastObj; i++)
    {
        if (!objects[i]->getSprite().empty())
        {
            //Might be dereferencing something you shouldn't.
            engine.graphics->RenderSprite(*objects[i]);
        }
    }


    engine.graphics->PostDraw();

    if (engine.phys->checkReset())
    {
        restartGame();
        engine.phys->patchReset();
    }

    //End of Frame Garbage collection
    WORLD->compactObjects(objects.GetNumActive());
    WORLD->compactObjects(col.GetNumActive());
    WORLD->compactColliders(behvs.GetNumActive());

    if (engine.gameInputs->getIsKeyDown(LILLIS::ESC))
    {
        engine.graphics->closeWindow();
    }
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
        case SINGLEIMAGES:
            ResourceManager::LoadTextureRecursive(filePath, true, false);
            break;
        case SPRITESHEET:
            ResourceManager::LoadTextureRecursive(filePath, true, true);
            break;
        case SHADERS:
            ResourceManager::LoadShaderRecursive(filePath);
            break;
        case SOUNDS:
            std::cout << "Not implemented yet" << '\n';
            break;
        case DATA:
            ResourceManager::LoadDataRecursive(filePath);
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
        case SINGLEIMAGES:
            ResourceManager::LoadTexture(filePath, true, false);
            break;
        case SPRITESHEET:
            ResourceManager::LoadTexture(filePath, true, true);
            break;
        case SHADERS:
            std::cerr << "SINGLE SHADER INSERTION PROHIBITED" << '\n';
            break;
        case SOUNDS:
            std::cout << "Not implemented yet" << '\n';
            break;
        case DATA:
            ResourceManager::LoadData(filePath);
            break;
        default:
            std::cerr << "INVALID ASSET INSERTION" << '\n';
            break;
    }
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