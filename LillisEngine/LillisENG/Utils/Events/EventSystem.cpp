#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

EventSystem* EventSystem::eventSystem = nullptr;

EventSystem::EventSystem(){}

EventSystem::~EventSystem()
{
	cleanup();
}

void EventSystem::addListener(EventType type, EventListener* pListener)
{
	if (IsInitted)
	{
		Listeners.insert(std::pair< EventType, EventListener* >(type, pListener));
	}
}

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

EventSystem* EventSystem::getInstance()
{
	if (eventSystem != nullptr)
	{
		return eventSystem;
	}
	return nullptr;
}

EventSystem* EventSystem::createInstance()
{
	if (eventSystem == nullptr)
	{
		eventSystem = new EventSystem;
	}
	return eventSystem;
}

void EventSystem::delInstance()
{
	delete eventSystem;
	eventSystem = nullptr;
}

void EventSystem::init()
{
	if (IsInitted)
	{
		cleanup();
	}
	IsInitted = true;
}

void EventSystem::cleanup()
{
	Listeners.clear();
	IsInitted = false;
}

void EventSystem::fireEvent(const Event& theEvent)
{
	if (IsInitted)
	{
		dispatchAllEvents(theEvent);
	}
}

void EventSystem::dispatchAllEvents(const Event& theEvent)
{
	if (IsInitted)
	{
		std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;
		ret = Listeners.equal_range(theEvent.getType());
		std::multimap<EventType, EventListener*>::iterator iter;
		for (iter = ret.first; iter != ret.second; ++iter)
		{
			iter->second->handleEvent(theEvent);
		}
	}
}