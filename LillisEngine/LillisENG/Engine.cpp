#include "Engine.h"

void Engine::Run()
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

    EngineState engine;
    engine.quit = false;
    engine.renderer = renderer;
    engine.frame = 0;
    engine.frameStart = GetTicks();
    engine.system = system;

    WORLD = DBG_NEW GameObjectManager();
    SceneLoader scl;
    scl.LoadData("Level.dat");

    runMainLoop(&engine);

    //WORLD->DelWorld();
    delete WORLD;
    delete FrameAllocator;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    system->Shutdown();
    delete system;
}

void Engine::runMainLoop(EngineState* engine)
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&frameStep, engine, 0, true);
#else
    while (!engine->quit)
    {
        Uint32 now = GetTicks();
        if (now - engine->frameStart >= 16)
        {
            frameStep(engine);
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

    for (int i = 0; i < WORLD->numPlayers; i++)
    {
        WORLD->players[i].Update(keyDown);
    }

    for (int i = 0; i < WORLD->numRotators; i++)
    {
        WORLD->rotators[i].Update(0.1);
    }

    for (int i = 0; i < WORLD->numColliders; i++)
    {
        WORLD->colliders[i].isCurrentlyColliding = false;
    }

    GameObject* colPoint = FrameAllocator->alloc<GameObject>(WORLD->numColliders);
    int colPointLoc = 0;

    for (int i = 0; i < WORLD->numColliders; i++)
    {
        for (int j = i + 1; j < WORLD->numColliders; j++)
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
    }
    if (colPointLoc > 0)
    {
        int num = 0;
        for (int i = 0; i < colPointLoc; i++)
        {
            num++;
        }
        //std::cout << num << " Objects" << std::endl;
    }

    for (int i = 0; i < WORLD->numRenderers; i++)
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