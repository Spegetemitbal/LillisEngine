#pragma once
#include "Event.h"
using namespace std;
enum EditorEventType
{
	LEVEL_CHANGE_EVENT = NUM_BASE_EVENT_TYPES,
	INPUT_EVENT,
	NUM_GAME_EVENT_TYPES
};
class EditorEvent :public Event
{
public:
	EditorEvent(EditorEventType type);
	virtual ~EditorEvent() {};
private:
};