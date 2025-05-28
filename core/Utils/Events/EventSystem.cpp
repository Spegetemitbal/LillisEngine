#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"
#include "../StackAllocator.h"

EventSystem* EventSystem::eventSystem = nullptr;

EventSystem::EventSystem()
{
	init();
}

EventSystem::~EventSystem()
{
	cleanup();
}

//Inserts a pair (eventtype/listener) into the event system
void EventSystem::addListener(EventType type, EventListener* pListener, unsigned int IDreq)
{
	if (IsInitted)
	{
		Listeners.insert(std::pair(type, ListenerStruct(pListener,IDreq)));
	}
}

void EventSystem::addCallback(EventType type, EventCallback pCallback, unsigned int IDreq)
{
	if (IsInitted)
	{
		Callbacks.insert(std::pair(type, CallbackStruct(pCallback,IDreq)));
	}
}

//iterates through the multimap and removes it from the list
void EventSystem::removeListener(EventType type, EventListener* pListener)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, ListenerStruct>::iterator, std::multimap<EventType, ListenerStruct>::iterator> ret;
		ret = Listeners.equal_range(type);
		std::multimap<EventType, ListenerStruct>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second.listener == pListener)
			{
				Listeners.erase(iter);
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::removeCallback(EventType type, EventCallback pCallback)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, CallbackStruct>::iterator, std::multimap<EventType, CallbackStruct>::iterator> ret;
		ret = Callbacks.equal_range(type);
		std::multimap<EventType, CallbackStruct>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second.callback == pCallback)
			{
				Callbacks.erase(iter);
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::clearIDreqFromCallback(EventType type, EventCallback pCallback)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, CallbackStruct>::iterator, std::multimap<EventType, CallbackStruct>::iterator> ret;
		ret = Callbacks.equal_range(type);
		std::multimap<EventType, CallbackStruct>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second.callback == pCallback)
			{
				iter->second.IDreq = 0;
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::clearIDreqFromListener(EventType type, EventListener *pListener)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, ListenerStruct>::iterator, std::multimap<EventType, ListenerStruct>::iterator> ret;
		ret = Listeners.equal_range(type);
		std::multimap<EventType, ListenerStruct>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second.listener == pListener)
			{
				iter->second.IDreq = 0;
				break;//to prevent using invalidated iterator
			}
		}
	}
}


//iterates through the multimap and removes the listener from every eventtype
void EventSystem::removeListenerFromAllEvents(EventListener* pListener)
{
	if (IsInitted)
	{
		std::multimap<EventType, ListenerStruct>::iterator iter;
		bool allTheWayThrough = false;
		while (!allTheWayThrough)
		{
			allTheWayThrough = true;
			for (iter = Listeners.begin(); iter != Listeners.end(); ++iter)
			{
				if (iter->second.listener == pListener)
				{
					Listeners.erase(iter);
					allTheWayThrough = false; //didn't make it the whole way through
					break;//to prevent using invalidated iterator
				}
			}
		}
	}
}

void EventSystem::removeCallbackFromAllEvents(EventCallback pCallback)
{
	if (IsInitted)
	{
		std::multimap<EventType, CallbackStruct>::iterator iter;
		bool allTheWayThrough = false;
		while (!allTheWayThrough)
		{
			allTheWayThrough = true;
			for (iter = Callbacks.begin(); iter != Callbacks.end(); ++iter)
			{
				if (iter->second.callback == pCallback)
				{
					Callbacks.erase(iter);
					allTheWayThrough = false; //didn't make it the whole way through
					break;//to prevent using invalidated iterator
				}
			}
		}
	}
}


//Singleton getter
EventSystem* EventSystem::getInstance()
{
	if (eventSystem == nullptr)
	{
		return createInstance();
	}
	return eventSystem;
}

//Singleton init
EventSystem* EventSystem::createInstance()
{
	if (eventSystem == nullptr)
	{
		eventSystem = DBG_NEW EventSystem;
	}
	return eventSystem;
}

//Singleton delete
void EventSystem::delInstance()
{
	if (eventSystem != nullptr)
	{
		delete eventSystem;
		eventSystem = nullptr;
	}
}

//Just marks as innited, wipes it if not.
void EventSystem::init()
{
	if (IsInitted)
	{
		cleanup();
	}
	IsInitted = true;
}

//wipes data structure.
void EventSystem::cleanup()
{
	Listeners.clear();
	Callbacks.clear();
	IsInitted = false;
}

//Checks for initialization and then just jumps to dispatch (perhaps simplify this)
//Make non-blocking at some point with an event-bus called all at once!
void EventSystem::fireEvent(const Event& theEvent)
{
	if (IsInitted)
	{
		dispatchAllEvents(theEvent);
	} else
	{
		std::cout << "EventSystem not initialized" << std::endl;
	}
}


//Iterates through all listeners of the specific type and sends them the event.
void EventSystem::dispatchAllEvents(const Event& theEvent)
{
	if (IsInitted)
	{

		//This whole first shenanigan is to make sure you're iterating through the part of the map with the correct type!
		std::pair<std::multimap<EventType, ListenerStruct>::iterator, std::multimap<EventType, ListenerStruct>::iterator> ret;
		ret = Listeners.equal_range(theEvent.getType());
		std::multimap<EventType, ListenerStruct>::iterator iter = ret.first;
		for (; iter != ret.second; ++iter)
		{
			if (iter->second.IDreq > 0)
			{
				if (theEvent.containsID(iter->second.IDreq))
				{
					iter->second.listener->handleEvent(theEvent);
				}
			} else
			{
				iter->second.listener->handleEvent(theEvent);
			}
		}

		//then for callbacks
		std::pair<std::multimap<EventType, CallbackStruct>::iterator, std::multimap<EventType, CallbackStruct>::iterator> ret2;
		ret2 = Callbacks.equal_range(theEvent.getType());
		auto iter2 = ret2.first;
		for (; iter2 != ret2.second; ++iter2)
		{
			if (iter2->second.IDreq > 0)
			{
				if (theEvent.containsID(iter2->second.IDreq))
				{
					iter2->second.callback(theEvent);
				}
			} else
			{
				iter2->second.callback(theEvent);
			}
		}
	}
}

void EventSystem::queueEvent(const Event &theEvent)
{
	//eventQueue.push();
}

void EventSystem::emptyQueue()
{
	while (!eventQueue.empty())
	{
		dispatchAllEvents(*eventQueue.front());
		eventQueue.pop();
	}
}

