#pragma once

#include "../Component.h"
#include "../../Utils/Events/EventSystem.h"
#include "../../Utils/Events/GameEvent.h"

class Behavior : public Component
{
public:
	Behavior(std::string name)
	{
		BehaviorName = name;
		if (IDList.find(name) != IDList.end())
		{
			BehaviorID = IDList[name];
		}
		else
		{
			BehaviorID = nextID;
			nextID++;
			IDList.emplace(name, BehaviorID);
		}
	}
	virtual ~Behavior() {};

	int getNextID() { return nextID; };
	
	//returns -1 if nonexistent
	static int getIDbyName(std::string n) 
	{
		if (IDList.find(n) != IDList.end())
		{
			return IDList[n];
		}
		else
		{
			return -1;
		}
	}
	void handleEvent(const Event& theEvent) {}

	//virtual void Update() = 0;

private:
	Behavior() {};
	bool isStatic = false;
	std::string BehaviorName = "";
	int BehaviorID = 0;

	static std::map<std::string, int> IDList;
	static int nextID;

protected:
	void addListener(GameEventType type);
};