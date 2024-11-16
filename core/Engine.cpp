#include "pch.h"
#include "Engine.h"

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

    engine.frame = 0;
    engine.frameStart = glfwGetTime();

    WORLD = DBG_NEW GameObjectManager();  

    restartGame();
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
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    do
    {
        //This logic is causing errors upon exiting!.
        double now = glfwGetTime();
        if (now - engine.frameStart >= 0.016)
        {
            frameStep();
        }
        engine.quit = engine.graphics->isWindowOpen();
    } while (!engine.quit);
#endif

}

//Wipe later
void Engine::restartGame()
{
    if (WORLD->numObjects > 0)
    {
        WORLD->clearAll();
    }

    //Game specifics
    p1 = WORLD->addObject(0, 0);
    p1->CreateCollider(40, 40, 0);
    p1->SetSprite("p1");
    p1->CreatePlayerController();

    p2 = WORLD->addObject(0, 440);
    p2->SetSprite("p2");
    p2->CreateCollider(40, 40, 1);
    p2->CreatePlayerController();

    GameObject* goal = WORLD->addObject(600, 220);
    goal->SetSprite("goal");
    goal->CreateCollider(40, 40, 2);

    float pos = 100;
    float ang = 20;
    for (int i = 0; i < 3; i++)
    {
        GameObject* spinny = WORLD->addObject(300, pos);
        spinny->SetSprite("enemy");
        spinny->CreateCollider(40, 40, 3);
        Rotator* r = spinny->CreateRotator(ang);
        r->setBaseOffset(300, pos);
        pos += 100;
        ang += 30;
    }

}


//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{
    double now = glfwGetTime();

    engine.frame++;
    engine.frameStart = now;

    vector<Rotator*> rot = WORLD->getRotators();

    unsigned int lastRotator = WORLD->getRotActive();
    for (int i = 0; i < lastRotator; i++)
    {
        if (rot[i]->isActive)
        {
            rot[i]->Update(0.1);
        }
    }

    vector<PlayerController*> pla = WORLD->getPlayers();

    unsigned int lastPlayer = WORLD->getPlayerActive();
    for (int i = 0; i < lastPlayer; i++)
    {
        if (pla[i]->isActive)
        {
            pla[i]->Update();
        }
    }

    vector<RectangleCollider*> col = WORLD->getColliders();

    unsigned int lastCol = WORLD->getColActive();
    for (int i = 0; i < lastCol; i++)
    {
        for (int j = i + 1; j < lastCol; j++)
        {
            if (col[j]->isActive && col[i]->isActive)
            {
                col[i]->CheckCollision(*col[j]);
            }
        }
    }

    engine.graphics->PreDraw();

    vector<GameObject*> objects = WORLD->getObjects();
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

//Call this once for each folder. If folder is not alphabetical, all hope be lost.
void Engine::InjectAssets(const char* filePath, AssetType resourceType)
{
    std::vector<std::string> folder = std::vector<std::string>();
    for (const auto& entry : fs::directory_iterator(filePath))
    {
        //std::cout << entry.path() << std::endl;
        folder.push_back(GetFileName(entry.path().string().data()));
    }

    // The container must be sorted!
    if (std::adjacent_find(folder.begin(), folder.end()) != folder.end())
    {
        std::cerr << "DUPLICATE FILES DETECTED" << '\n';
        return;
    }

    switch (resourceType)
    {
    case SINGLEIMAGES:
        break;
    case SPRITESHEET:
        break;
    case SHADERS:
        break;
    case SOUNDS:
        break;
    default:
        std::cerr << "INVALID ASSET INSERTION" << '\n';
        break;
    }

    std::cout << "Multiple asset injection not ready yet :(" << '\n';
}

void Engine::InjectSingleAsset(const char* filePath, AssetType resourceType)
{
    std::string FileName = GetFileName(filePath);

    switch (resourceType)
    {
    case SINGLEIMAGES:
        break;
    case SPRITESHEET:
        break;
    case SHADERS:
        std::cerr << "SINGLE SHADER INSERTION PROHIBITED" << '\n';
        break;
    case SOUNDS:
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