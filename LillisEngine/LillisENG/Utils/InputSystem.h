#pragma once

#include "Events/EventSystem.h"
#include "Events/InputEvent.h"
#include <SDL.h>

class InputSystem
{
public:
	InputSystem() {};
	~InputSystem() {};
	bool init();
	void cleanup();
	void update();
	//int getMouseState();
	//Vector2 getMousePosition();
};
