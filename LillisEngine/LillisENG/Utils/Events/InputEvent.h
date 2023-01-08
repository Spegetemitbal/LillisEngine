#pragma once

#include "GameEvent.h"
class InputEvent : public GameEvent
{
public:
	InputEvent(const int& inputCode);
	~InputEvent() {};
	int getInput() const { return mInputCode; };
private:
	int mInputCode;
};