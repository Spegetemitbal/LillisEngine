#include "pch.h"
#include "InputEvent.h"

InputEvent::InputEvent(const int& inputCode, const bool& down) : GameEvent(INPUT_EVENT)
{
	mInputCode = inputCode;
	mDown = down;
}