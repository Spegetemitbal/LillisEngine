#include "Engine.h"

Engine::Engine()
{
    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    GPR460::System* system = GPR460::System::Create();
    system->Init();

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_GetKeyboardState(0);

    currentState = DBG_NEW EngineState();
    currentState->quit = false;
    currentState->window = window;
    currentState->renderer = renderer;
    currentState->frame = 0;
    currentState->frameStart = GetTicks();
    currentState->system = system;

    WORLD = DBG_NEW GameObjectManager();
}

Engine::~Engine()
{
    //WORLD->DelWorld();
    delete WORLD;
    delete FrameAllocator;

    SDL_DestroyRenderer(currentState->renderer);
    SDL_DestroyWindow(currentState->window);
    SDL_Quit();

    currentState->system->Shutdown();
    delete currentState->system;

    delete currentState;
}

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

void Engine::Run()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    while (!currentState->quit)
    {
        Uint32 now = GetTicks();
        if (now - currentState->frameStart >= 16)
        {
            frameStep(currentState);
        }
    }
#endif
}

void Engine::frameStep(void* arg)
{
    EngineState* engine = (EngineState*)arg;
    SDL_Event event;

    Uint32 now = GetTicks();

    engine->frame++;
    engine->frameStart = now;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            engine->quit = true;
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                engine->quit = true;
            }
        }
    }

    const Uint8* keyDown = SDL_GetKeyboardState(0);

    for (int i = 0; i < WORLD->players.size(); i++)
    {
        WORLD->players[i].Update(keyDown);
    }

    for (int i = 0; i < WORLD->rotators.size(); i++)
    {
        WORLD->rotators[i].Update(0.1);
    }

    /*for (int i = 0; i < WORLD->colliders.size(); i++)
    {
        WORLD->colliders[i].isCurrentlyColliding = false;
    }*/

    GameObject* colPoint = FrameAllocator->alloc<GameObject>(WORLD->colliders.size());
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

    //Render
    for (int i = 0; i < WORLD->renderers.size(); i++)
    {
        Color c = WORLD->renderers[i].getColor();
        Transform t = WORLD->renderers[i].getObject()->transform;
        SDL_SetRenderDrawColor(engine->renderer, c.getR(), c.getG(), c.getB(), c.getA());
        SDL_Rect r = { t.x, t.y,  WORLD->renderers[i].getWidth(),  WORLD->renderers[i].getHeight() };
        SDL_RenderFillRect(engine->renderer, &r);
    }

    SDL_RenderPresent(engine->renderer);
    FrameAllocator->clearStack();
}

Uint32 Engine::GetTicks()
{
    return SDL_GetTicks();
}

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

Engine* Engine::CreateGameInstance()
{
    if (Game == nullptr)
    {
        Game = new Engine();
    }
    return Game;
}

void Engine::DestroyGameInstance()
{
    delete Game;
    Game = nullptr;
}

Engine* Engine::Game = nullptr;