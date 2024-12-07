#pragma once

#include "../Utils/Events/EventSystem.h"
#include "../Utils/Events/EventListener.h"

class GameObject;

//Base class for components
class Component : public EventListener
{
public:
	Component() {entityID = nextID; nextID++;};
	virtual ~Component() {};

	GameObject* getObject() { return thisObject; }
	void setControlledObject(GameObject* g) { thisObject = g; }

	unsigned int GetId() const { return entityID; }
	void handleEvent(const Event& theEvent) {}

	//If inactive, is garbage data
	bool isActive = false;
	//If disabled, is not garbage data, but won't be read.
	bool isEnabled = false;

protected:
	GameObject* thisObject = nullptr;
private:
	unsigned int entityID = 0;
	static unsigned int nextID;
};