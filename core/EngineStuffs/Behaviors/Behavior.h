#pragma once

#include "../Component.h"
#include "../../Utils/Events/EventSystem.h"
#include "../../Utils/Events/GameEvent.h"

class Behavior : public Component
{
public:

	Behavior() = delete;

	//Do not add listeners here.
	Behavior(std::string name)
	{
		BehaviorName = name;
	}
	virtual ~Behavior() {};

	std::string GetName() { return BehaviorName; }

	void handleEvent(const Event& theEvent) {}

	//Add listeners here.
	virtual void LoadListeners() = 0;
	//Updates every frame.
	virtual void Update(float deltaTime) = 0;

private:
	std::string BehaviorName = "";

protected:
	void addListener(GameEventType type);
};