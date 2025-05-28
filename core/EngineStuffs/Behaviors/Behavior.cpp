#include "Behavior.h"

void Behavior::addListener(GameEventType type, unsigned int IDreq)
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)type, this, IDreq);
}