#include "Behavior.h"

void Behavior::addListener(GameEventType type)
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)type, this);
}

int Behavior::nextID = 0;

std::map<std::string, int> Behavior::IDList = map<std::string,int>();