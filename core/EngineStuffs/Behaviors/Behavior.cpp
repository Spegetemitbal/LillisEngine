#include "Behavior.h"

void Behavior::addListener(GameEventType type)
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)type, this);
}