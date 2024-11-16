#include "pch.h"
#include "EditorEvent.h"
EditorEvent::EditorEvent(EditorEventType type)
	:Event((EventType)type)
{
}