#pragma once

#include "../Utils/Events/EventSystem.h"
#include "../Utils/Events/EventListener.h"

class GameObject;

//Base class for components
class Component : public EventListener
{
public:
	Component() {};
	~Component() {};

	GameObject* getObject() { return thisObject; }
	void setControlledObject(GameObject* g) { thisObject = g; }

	void handleEvent(const Event& theEvent) {}

protected:
	GameObject* thisObject = nullptr;
};