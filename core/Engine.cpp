#include "Engine.h"

#include "Utils/Timing.h"

//Creates window, Initializes low level systems and loads scene.
Engine::Engine()
{
    engine = EngineState();
    engine.quit = false;

    engine.system = System::Create();
    engine.system->Init();

    EventSystem* ev = EventSystem::createInstance();
    ev->init();
}

//Clears everything
Engine::~Engine()
{
    //WORLD->DelWorld();
    delete WORLD;
    //delete FrameAllocator

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
    if (engine.isRunning)
    {
        engine.nextLevel = Data;
        engine.loadNextLevel = true;
    } else
    {
        SceneLoad(Data);
    }
}

void Engine::SceneLoad(std::string Data)
{
    if (WORLD == nullptr)
    {
        WORLD = DBG_NEW GameObjectManager();
    }
    else
    {
        WORLD->clearAll();
    }
    SceneLoader::LoadData(Data);
    CurrentLevel = Data;
}


//Game loop
void Engine::Run()
{
    engine.isRunning = true;
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


//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{
    if (engine.loadNextLevel)
    {
        SceneLoad(engine.nextLevel);
        engine.loadNextLevel = false;
    }

    WORLD->RunTransformHierarchy();

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

    ActiveTracker<Sprite*> sprites = WORLD->getSpritesRaw();
    unsigned int lastSpr = WORLD->getSprActive();
    for (int i = 0; i < lastSpr; i++)
    {
        if (sprites[i]->GetActive())
        {
            //Might be dereferencing something you shouldn't.
            engine.graphics->RenderSprite(*sprites[i]);
        }
    }


    engine.graphics->PostDraw();

    //End of Frame Garbage collection
    WORLD->compactObjects(sprites.GetNumActive());
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
        case SPRITE:
            ResourceManager::LoadTextureRecursive(filePath, true);
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
        case SPRITE:
            ResourceManager::LoadTexture(filePath, true);
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

void Engine::LoadImportData(const char *filePath)
{
    ResourceManager::LoadImportInfo(filePath);
}


void Engine::Init(int screenWidth, int screenHeight, std::string gameName, std::vector<LILLIS::KeyCode> keys)
{

    engine.graphics = DBG_NEW GraphicsSystem(screenWidth, screenHeight, gameName);
    if (!engine.graphics->Init())
    {
        exit(1);
    }

    engine.gameInputs = DBG_NEW InputSystem(keys);
    engine.gameInputs->setupKeyInputs(engine.graphics->GetWin());

    Timing::Init();

    WORLD = DBG_NEW GameObjectManager();
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