#pragma once

#include "Events/EventSystem.h"
#include "Events/InputEvent.h"
#include <SDL.h>

class Engine;

enum KeyCode
{
	Q = 10,
	W, E, R, T, Y, U, I, O, P, L_BRACKET, R_BRACKET, BACKSLASH,
	A, S, D, F, G, H, J, K, L, SEMICOLON, QUOTATION, ENTER,
	L_SHIFT, Z, X, C, V, B, N, M, COMMA, PERIOD, SLASH, R_SHIFT,
	L_CTRL, L_ALT, SPACE, R_ALT, R_CTRL, LEFT, RIGHT, UP, DOWN,
	ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	SQUIGGLE, DASH_MINUS, PLUS_EQUALS, BACKSPACE
};

class InputSystem
{
public:

	friend class Engine;

	static InputSystem* Input;

	bool getKey(KeyCode k);
	bool getKeyDown(KeyCode k);
	bool getKeyUp(KeyCode k);

	static InputSystem* GetSystemInstance();
	static InputSystem* CreateSystemInstance();
	static void DestroySystemInstance();
	//int getMouseState();
	//Vector2 getMousePosition();
private:

	void tick();

	InputSystem();
	~InputSystem() { 
		delete previousState; delete keyMap; delete conversionMap;
		keyMap = nullptr; previousState = nullptr; conversionMap = nullptr;
	};

	bool* keyMap = nullptr;
	bool* previousState = nullptr;
	Uint8* conversionMap = nullptr;
};
