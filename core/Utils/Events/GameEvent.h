#pragma once
#include "Event.h"
using namespace std;

enum GameEventType
{
	LEVEL_CHANGE_EVENT = NUM_BASE_EVENT_TYPES,
	INPUT_EVENT,
	MOUSE_INPUT_EVENT,
	CONTROLLER_BUTTON_EVENT,
	CONTROLLER_AXIS_EVENT,
	COLLISION_EVENT,
	STATE_CHANGE_EVENT,
	NUM_GAME_EVENT_TYPES
};

class GameEvent :public Event
{
public:
	GameEvent(GameEventType type);
	virtual ~GameEvent() {};
private:
};