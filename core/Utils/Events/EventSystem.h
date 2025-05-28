#pragma once

#include <queue>

#include "GameEvent.h"

class Event;
class EventListener;
enum EventType;
enum GameEventType;

typedef void( *EventCallback )(const Event& theEvent);

struct CallbackStruct
{
	CallbackStruct(EventCallback theCallback, unsigned int ID)
	{
		callback = theCallback;
		IDreq = ID;
	}
	EventCallback callback = nullptr;
	unsigned int IDreq = 0;
};

struct ListenerStruct
{
	ListenerStruct(EventListener* theListener, unsigned int ID)
	{
		listener = theListener;
		IDreq = ID;
	}
	EventListener* listener = nullptr;
	unsigned int IDreq = 0;
};

class EventSystem
{
public:
	static EventSystem* getInstance();
	static EventSystem* createInstance();
	static void delInstance();

	void init();
	void cleanup();
	void fireEvent(const Event& theEvent);
	void addListener(EventType type, EventListener* pListener, unsigned int IDreq = 0);
	void addCallback(EventType type, EventCallback pCallback, unsigned int IDreq = 0);
	void clearIDreqFromListener(EventType type, EventListener* pListener);
	void clearIDreqFromCallback(EventType type, EventCallback pCallback);
	void removeListener(EventType type, EventListener* pListener);
	void removeCallback(EventType type, EventCallback pCallback);
	void removeListenerFromAllEvents(EventListener* pListener);
	void removeCallbackFromAllEvents(EventCallback pCallback);
	void queueEvent(const Event& theEvent);
	void emptyQueue();
	//Further implementation later

private:
	static EventSystem* eventSystem;

	std::queue<Event*> eventQueue;

	std::multimap< EventType, ListenerStruct> Listeners;
	std::multimap< EventType, CallbackStruct> Callbacks;
	bool IsInitted = false;
	void dispatchAllEvents(const Event& theEvent);
	EventSystem();
	~EventSystem();
};