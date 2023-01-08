
#include "InputEvent.h"
InputEvent::InputEvent(const int& inputCode) : GameEvent(INPUT_EVENT)
{
	mInputCode = inputCode;
}