#pragma once

class GameObject;

//Base class for components
class Component
{
public:
	Component() {};
	~Component() {};

	GameObject* getObject() { return thisObject; }
	void setControlledObject(GameObject* g) { thisObject = g; }

protected:
	GameObject* thisObject = nullptr;
};