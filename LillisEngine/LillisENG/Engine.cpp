#include "Engine.h"
#include "Utils/InputSystem.h"

//Creates window, Initializes low level systems and loads scene.
Engine::Engine()
{
    EventSystem::createInstance();

    const int WIDTH = 640;
    const int HEIGHT = 480;


    engine = EngineState();
    engine.quit = false;

    engine.graphics = new GraphicsSystem(WIDTH, HEIGHT, "Game");
    engine.graphics->Init();
    engine.frame = 0;
    engine.frameStart = glfwGetTime();
    engine.system = LILLIS::System::Create();
    engine.system->Init();

    //Leaky abstraction. Either the inputsystem should pass in the graphicssystem, or the graphicssystem should pass in the inputsystem.
    InputSystem::CreateSystemInstance(engine.graphics->getWindow());

    //SDL_Init(SDL_INIT_VIDEO);
    //SDL_GetKeyboardState(0);

    WORLD = DBG_NEW GameObjectManager();
}

//Clears everything
Engine::~Engine()
{
    //WORLD->DelWorld();
    delete WORLD;
    //delete FrameAllocator;

    EventSystem::delInstance();
    InputSystem::DestroySystemInstance();

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
    while (!engine.quit)
    {
        double now = glfwGetTime();
        if (now - engine.frameStart >= 0.016)
        {
            frameStep();
        }
        engine.quit = engine.graphics->isWindowOpen();
    }
#endif
}

//Occurs every frame, the 'content' of the game loop
void Engine::frameStep()
{
    double now = glfwGetTime();

    engine.frame++;
    engine.frameStart = now;

    InputSystem* inSys = InputSystem::GetSystemInstance();
    inSys->tick();

    engine.graphics->Update();

    for (int i = 0; i < WORLD->players.size(); i++)
    {
        WORLD->players[i].Update();
    }

    for (int i = 0; i < WORLD->rotators.size(); i++)
    {
        WORLD->rotators[i].Update(0.1);
    }

    /*for (int i = 0; i < WORLD->objects.size(); i++)
    {
        for (int j = 0; j < WORLD->objects[i].behaviors.size(); j++)
        {
            WORLD->objects[i].behaviors.Update();
        }
    } */

    /*for (int i = 0; i < WORLD->colliders.size(); i++)
    {
        WORLD->colliders[i].isCurrentlyColliding = false;
    }*/

    //GameObject* colPoint = FrameAllocator->alloc<GameObject>(WORLD->colliders.size());
    int colPointLoc = 0;

    //Collision
    /*for (int i = 0; i < WORLD->colliders.size(); i++)
    {
        for (int j = i + 1; j < WORLD->colliders.size(); j++)
        {
            if (WORLD->colliders[i].CheckCollision(WORLD->colliders[j]))
            {
                WORLD->colliders[i].isCurrentlyColliding = true;
                WORLD->colliders[j].isCurrentlyColliding = true;
                if (i != 7 && j == j)
                {
                    GameObject* g = WORLD->colliders[i].getObject();
                    colPoint[colPointLoc] = *g;
                    colPointLoc++;
                }
            }
        }
    } */
    if (colPointLoc > 0)
    {
        int num = 0;
        for (int i = 0; i < colPointLoc; i++)
        {
            num++;
        }
        //std::cout << num << " Objects" << std::endl;
    }
    //FrameAllocator->clearStack();
}

//Returns time
//Uint32 Engine::GetTicks()
//{
//    return SDL_GetTicks();
//}

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