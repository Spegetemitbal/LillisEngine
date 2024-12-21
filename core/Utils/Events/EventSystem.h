#pragma once

#include <queue>

#include "GameEvent.h"

//Note for improvement: Change the data structure, multimap is SUPER unintuitive.

class Event;
class EventListener;
enum EventType;
enum GameEventType;

typedef void( *EventCallback )(const Event& theEvent);

class EventSystem
{
public:
	static EventSystem* getInstance();
	static EventSystem* createInstance();
	static void delInstance();

	void init();
	void cleanup();
	void fireEvent(const Event& theEvent);
	void addListener(EventType type, EventListener* pListener);
	void addCallback(EventType type, EventCallback pCallback);
	void removeListener(EventType type, EventListener* pListener);
	void removeCallback(EventType type, EventCallback pCallback);
	void removeListenerFromAllEvents(EventListener* pListener);
	void removeCallbackFromAllEvents(EventCallback pCallback);

private:
	static EventSystem* eventSystem;
	//Swapping over because br0ke
	std::multimap< EventType, EventListener* > Listeners;
	std::multimap< EventType, EventCallback > Callbacks;
	bool IsInitted = false;
	void dispatchAllEvents(const Event& theEvent);
	EventSystem();
	~EventSystem();
};