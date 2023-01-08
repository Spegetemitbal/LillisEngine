#include "InputSystem.h"
bool InputSystem::init()
{
	/*if (SDL_Init(SDL_INIT_EVENTS) != 0)
	{
		std::cout << "Issues with keyboard." << std::endl;
		system("pause");
		return false;
	} */
	return true;
}
void InputSystem::cleanup()
{
	//SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void InputSystem::update()
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	SDL_Event events;
	int which_key = 0;
	if (SDL_PollEvent(&events) != 0)
	{
		if (events.type == SDL_KEYDOWN)
		{
			which_key = events.key.keysym.sym;
		}
	}
	switch (which_key)
	{
	case SDLK_w:
		pEventSystem->fireEvent(InputEvent(1));
		break;
	case SDLK_s:
		pEventSystem->fireEvent(InputEvent(2));
		break;
	case SDLK_a:
		pEventSystem->fireEvent(InputEvent(3));
		break;
	case SDLK_d:
		pEventSystem->fireEvent(InputEvent(4));
		break;
	case SDLK_ESCAPE:
		pEventSystem->fireEvent(InputEvent(9));
		break;
	case SDLK_UP:
		pEventSystem->fireEvent(InputEvent(5));
		break;
	case SDLK_DOWN:
		pEventSystem->fireEvent(InputEvent(6));
		break;
	case SDLK_LEFT:
		pEventSystem->fireEvent(InputEvent(7));
		break;
	case SDLK_RIGHT:
		pEventSystem->fireEvent(InputEvent(8));
		break;
	case SDLK_p:
		pEventSystem->fireEvent(InputEvent(10));
		break;
	case SDLK_SPACE:
		pEventSystem->fireEvent(InputEvent(11));
		break;
	case SDLK_m:
		pEventSystem->fireEvent(InputEvent(12));
		break;
	case SDLK_n:
		pEventSystem->fireEvent(InputEvent(13));
		break;
	case SDLK_l:
		pEventSystem->fireEvent(InputEvent(14));
		break;
	case SDLK_k:
		pEventSystem->fireEvent(InputEvent(15));
		break;
	default:
		break;
	}
}