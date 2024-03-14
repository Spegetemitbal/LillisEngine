#pragma once

#include "GameEvent.h"

enum KeyCode
{
	KEY0, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9,
	Q, W, E, R, T, Y, U, I, O, P, L_BRACKET, R_BRACKET, BACKSLASH,
	A, S, D, F, G, H, J, K, L, SEMICOLON, QUOTATION, ENTER,
	L_SHIFT, Z, X, C, V, B, N, M, COMMA, PERIOD, SLASH, R_SHIFT,
	L_CTRL, L_ALT, SPACE, R_ALT, R_CTRL, LEFT, RIGHT, UP, DOWN,
	ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	SQUIGGLE, DASH_MINUS, PLUS_EQUALS, BACKSPACE
};

class InputEvent : public GameEvent
{
public:
	InputEvent(const int& inputCode, const bool& down);
	~InputEvent() {};
	int getInput() const { return mInputCode; };
	bool getPressed() const { return mDown; };
private:
	int mInputCode;
	bool mDown;
};