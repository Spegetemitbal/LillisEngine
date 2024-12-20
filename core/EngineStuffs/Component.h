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

	//An inactive object is garbage and will be wiped.
	void SetActive(bool active);
	bool GetActive() const { return isActive; };

	//A disabled object is not garbage and will not be wiped.
	void SetEnabled(bool enabled) {if (isActive) {isEnabled = enabled;} };
	bool GetEnabled() const { return isEnabled; };

protected:
	LilObj<GameObject> thisObject;
	//If inactive, is garbage data
	bool isActive = false;
	//If disabled, is not garbage data, but won't be read.
	bool isEnabled = false;

	//Perhaps add a map of these or some way to have a different string of IDs for each component!
	inline static unsigned int nextID = 1;
private:
	unsigned int entityID = 0;
};