#include "InputSystem.h"

void InputSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << key << " " << action << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

InputSystem::InputSystem(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);

	keyMap = DBG_NEW bool[72]; 
	previousState = DBG_NEW bool[72];
	//conversionMap = DBG_NEW Uint8[72];

	for (int i = 0; i < 72; i++)
	{
		keyMap[i] = false;
		previousState[i] = false;
	}

	/*
	conversionMap[0] = SDL_SCANCODE_0;
	conversionMap[1] = SDL_SCANCODE_1;
	conversionMap[2] = SDL_SCANCODE_2;
	conversionMap[3] = SDL_SCANCODE_3;
	conversionMap[4] = SDL_SCANCODE_4;
	conversionMap[5] = SDL_SCANCODE_5;
	conversionMap[6] = SDL_SCANCODE_6;
	conversionMap[7] = SDL_SCANCODE_7;
	conversionMap[8] = SDL_SCANCODE_8;
	conversionMap[9] = SDL_SCANCODE_9;
	conversionMap[10] = SDL_SCANCODE_Q;
	conversionMap[11] = SDL_SCANCODE_W;
	conversionMap[12] = SDL_SCANCODE_E;
	conversionMap[13] = SDL_SCANCODE_R;
	conversionMap[14] = SDL_SCANCODE_T;
	conversionMap[15] = SDL_SCANCODE_Y;
	conversionMap[16] = SDL_SCANCODE_U;
	conversionMap[17] = SDL_SCANCODE_I;
	conversionMap[18] = SDL_SCANCODE_O;
	conversionMap[19] = SDL_SCANCODE_P;
	conversionMap[20] = SDL_SCANCODE_LEFTBRACKET;
	conversionMap[21] = SDL_SCANCODE_RIGHTBRACKET;
	conversionMap[22] = SDL_SCANCODE_A;
	conversionMap[23] = SDL_SCANCODE_S;
	conversionMap[24] = SDL_SCANCODE_D;
	conversionMap[25] = SDL_SCANCODE_F;
	conversionMap[26] = SDL_SCANCODE_G;
	conversionMap[27] = SDL_SCANCODE_H;
	conversionMap[28] = SDL_SCANCODE_I;
	conversionMap[29] = SDL_SCANCODE_J;
	conversionMap[30] = SDL_SCANCODE_K;
	conversionMap[31] = SDL_SCANCODE_L;
	conversionMap[32] = SDL_SCANCODE_SEMICOLON;
	conversionMap[33] = SDL_SCANCODE_APOSTROPHE;
	conversionMap[34] = SDL_SCANCODE_LSHIFT;
	conversionMap[35] = SDL_SCANCODE_Z;
	conversionMap[36] = SDL_SCANCODE_X;
	conversionMap[37] = SDL_SCANCODE_C;
	conversionMap[38] = SDL_SCANCODE_V;
	conversionMap[39] = SDL_SCANCODE_B;
	conversionMap[40] = SDL_SCANCODE_N;
	conversionMap[41] = SDL_SCANCODE_M;
	conversionMap[42] = SDL_SCANCODE_COMMA;
	conversionMap[43] = SDL_SCANCODE_PERIOD;
	conversionMap[44] = SDL_SCANCODE_SLASH;
	conversionMap[45] = SDL_SCANCODE_RSHIFT;
	conversionMap[46] = SDL_SCANCODE_LCTRL;
	conversionMap[47] = SDL_SCANCODE_LALT;
	conversionMap[48] = SDL_SCANCODE_SPACE;
	conversionMap[49] = SDL_SCANCODE_RALT;
	conversionMap[50] = SDL_SCANCODE_RCTRL;
	conversionMap[51] = SDL_SCANCODE_LEFT;
	conversionMap[52] = SDL_SCANCODE_RIGHT;
	conversionMap[53] = SDL_SCANCODE_UP;
	conversionMap[54] = SDL_SCANCODE_DOWN;
	conversionMap[55] = SDL_SCANCODE_ESCAPE;
	conversionMap[56] = SDL_SCANCODE_F1;
	conversionMap[57] = SDL_SCANCODE_F2;
	conversionMap[58] = SDL_SCANCODE_F3;
	conversionMap[59] = SDL_SCANCODE_F4;
	conversionMap[60] = SDL_SCANCODE_F5;
	conversionMap[61] = SDL_SCANCODE_F6;
	conversionMap[62] = SDL_SCANCODE_F7;
	conversionMap[63] = SDL_SCANCODE_F8;
	conversionMap[64] = SDL_SCANCODE_F9;
	conversionMap[65] = SDL_SCANCODE_F10;
	conversionMap[66] = SDL_SCANCODE_F11;
	conversionMap[67] = SDL_SCANCODE_F12;
	conversionMap[68] = SDL_SCANCODE_GRAVE;
	conversionMap[69] = SDL_SCANCODE_MINUS;
	conversionMap[70] = SDL_SCANCODE_EQUALS;
	conversionMap[71] = SDL_SCANCODE_BACKSPACE;
	*/
}

void InputSystem::tick()
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	//SDL_Event events;

	//const Uint8* keyDown = SDL_GetKeyboardState(0);

	//replace this
	/*for (int i = 0; i < 72; i++)
	{
		bool changed = false;

		previousState[i] = keyMap[i];
		keyMap[i] = false;

		if (keyDown[conversionMap[i]])
		{
			keyMap[i] = true;			
		}

		if (keyMap[i] != previousState[i])
		{
			//std::cout << "Fired Input" << std::endl;
			//Note to self: add some kind of accessible data structure in the scripting system to distinguish ondown vs held.
			pEventSystem->fireEvent(InputEvent(KeyCode(i), keyMap[i]));
		}
	}*/
}

InputSystem* InputSystem::GetSystemInstance()
{
	if (Input != nullptr)
	{
		return Input;
	}
	else
	{
		return nullptr;
	}
}

InputSystem* InputSystem::CreateSystemInstance(GLFWwindow* w)
{
	if (Input == nullptr)
	{
		Input = DBG_NEW InputSystem(w);
	}
	return Input;
}

void InputSystem::DestroySystemInstance()
{
	delete Input;
	Input = nullptr;
}

InputSystem* InputSystem::Input = nullptr;