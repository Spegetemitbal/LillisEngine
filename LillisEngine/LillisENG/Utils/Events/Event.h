#pragma once

using namespace std;
enum EventType
{
	INVALID_EVENT_TYPE = -1,
	NUM_BASE_EVENT_TYPES
};
class Event
{
public:
	Event(EventType type);
	virtual ~Event();
	EventType getType() const { return mType; };
private:
	EventType mType;
};