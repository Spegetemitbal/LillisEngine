#pragma once

#include "../Utils/Events/EventSystem.h"
#include "../Utils/Events/EventListener.h"
#include "MemoryManagement/LilObj.h"

class GameObject;

//Base class for components
class Component : public EventListener
{
public:
	Component() {entityID = nextID; nextID++;};
	virtual ~Component() {};

	LilObj<GameObject> getObject() { return thisObject; }
	void setControlledObject(LilObj<GameObject> g) { thisObject = g; }

	unsigned int GetID() const { return entityID; }
	void handleEvent(const Event& theEvent) {}

	//If inactive, is garbage data
	bool isActive = false;
	//If disabled, is not garbage data, but won't be read.
	bool isEnabled = false;

protected:
	LilObj<GameObject> thisObject;
private:
	unsigned int entityID = 0;
	//Perhaps add a map of these or some way to have a different string of IDs for each component!
	static unsigned int nextID;
};