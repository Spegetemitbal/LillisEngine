#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

EventSystem* EventSystem::eventSystem = nullptr;

EventSystem::EventSystem()
{

}

EventSystem::~EventSystem()
{
	cleanup();
}

//Inserts a pair (eventtype/listener) into the event system
void EventSystem::addListener(EventType type, EventListener* pListener)
{
	if (IsInitted)
	{
		Listeners.insert(std::pair< EventType, EventListener* >(type, pListener));
	}
}

//iterates through the multimap and removes it from the list
void EventSystem::removeListener(EventType type, EventListener* pListener)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;
		ret = Listeners.equal_range(type);
		std::multimap<EventType, EventListener*>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second == pListener)
			{
				Listeners.erase(iter);
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
		std::multimap<EventType, EventListener*>::iterator iter;
		bool allTheWayThrough = false;
		while (!allTheWayThrough)
		{
			allTheWayThrough = true;
			for (iter = Listeners.begin(); iter != Listeners.end(); ++iter)
			{
				if (iter->second == pListener)
				{
					Listeners.erase(iter);
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
	if (eventSystem != nullptr)
	{
		return eventSystem;
	}
	return nullptr;
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
	delete eventSystem;
	eventSystem = nullptr;
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
	IsInitted = false;
}

//Checks for initialization and then just jumps to dispatch (perhaps simplify this)
void EventSystem::fireEvent(const Event& theEvent)
{
	if (IsInitted)
	{
		dispatchAllEvents(theEvent);
	}
}

//Iterates through all listeners of the specific type and sends them the event.
void EventSystem::dispatchAllEvents(const Event& theEvent)
{
	if (IsInitted)
	{
		//This whole first shenanigan is to make sure you're iterating through the part of the map with the correct type!
		std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;
		ret = Listeners.equal_range(theEvent.getType());
		std::multimap<EventType, EventListener*>::iterator iter = ret.first;
		for (; iter != ret.second; ++iter)
		{
			iter->second->handleEvent(theEvent);
		}
	}
}