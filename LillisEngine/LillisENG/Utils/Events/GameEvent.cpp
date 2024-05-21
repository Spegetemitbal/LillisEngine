#include "pch.h"
#include "GameEvent.h"
GameEvent::GameEvent(GameEventType type)
	:Event((EventType)type)
{
}