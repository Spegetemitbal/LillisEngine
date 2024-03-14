#include "../../pch.h"
#pragma once

//Note for improvement: Change the data structure, multimap is SUPER unintuitive.

class Event;
class EventListener;
enum EventType;

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
	void removeListener(EventType type, EventListener* pListener);
	void removeListenerFromAllEvents(EventListener* pListener);
private:
	static EventSystem* eventSystem;
	std::multimap< EventType, EventListener* > Listeners;
	bool IsInitted = false;
	void dispatchAllEvents(const Event& theEvent);
	EventSystem();
	~EventSystem();
};