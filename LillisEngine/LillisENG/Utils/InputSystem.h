#pragma once

#include "Events/EventSystem.h"
#include "Events/InputEvent.h"
#include <glfw3.h>

class Engine;

class InputSystem
{
public:

	friend class Engine;

	static InputSystem* Input;

	static InputSystem* GetSystemInstance();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//int getMouseState();
	//Vector2 getMousePosition();
private:

	void tick();

	//The engine itself will be the only thing allowed to make or destroy this
	static InputSystem* CreateSystemInstance(GLFWwindow* w);
	static void DestroySystemInstance();

	InputSystem() {}
	InputSystem(GLFWwindow* w);
	~InputSystem() { 
		delete previousState; delete keyMap; //delete conversionMap;
		keyMap = nullptr; previousState = nullptr; //conversionMap = nullptr;
	};

	bool* keyMap = nullptr;
	bool* previousState = nullptr;
	//Uint8* conversionMap = nullptr;
};
