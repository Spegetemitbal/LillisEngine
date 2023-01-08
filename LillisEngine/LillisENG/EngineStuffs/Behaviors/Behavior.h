#pragma once

#include "../Component.h"

class Behavior : public Component
{
public:
	Behavior();
	~Behavior();

	int BehaviorID = 0;

private:
	bool isStatic = false;
	int BehaviorID = 0;
};