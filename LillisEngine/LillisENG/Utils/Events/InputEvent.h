#pragma once

#include "GameEvent.h"

namespace LILLIS
{
    enum KeyCode
    {
        KEY0 = 48, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9,
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        L_BRACKET = 91, R_BRACKET = 93,
        BACKSLASH = 92,
        SEMICOLON = 59, QUOTATION = 39,
        ENTER = 257,
        L_SHIFT = 340,
        COMMA = 44, PERIOD = 46,
        SLASH = 47, R_SHIFT = 344,
        L_CTRL = 341, L_ALT, SPACE = 32, R_ALT = 346,
        R_CTRL = 345, LEFT = 263, RIGHT = 262, UP = 265, DOWN = 264,
        ESC = 256, F1 = 290, F2, F3, F4,
        F5, F6, F7, F8, F9, F10, F11, F12,
        SQUIGGLE = 96, DASH_MINUS = 45, PLUS_EQUALS = 61, BACKSPACE = 259,
        TAB = 258
    };
}

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