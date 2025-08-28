#pragma once

#include "../Component.h"
#include "../../Utils/Events/EventSystem.h"
#include "../../Utils/Events/GameEvent.h"
#include "EngineStuffs/MemoryManagement/Serializer.h"

class Behavior : public Component
{
public:

	Behavior() = delete;

	Behavior(std::string name)
	{
		BehaviorName = name;
	}
	virtual ~Behavior() {};

	Behavior(const Behavior& behavior) = default;

	void handleEvent(const Event& theEvent) {}

	std::string GetName() { return BehaviorName; }

	//TODO add a disabler to ensure behaviors with no updates or lateupdates don't get checked.
	//Pre physics / other game logic.
	virtual void Update(float deltaTime) {};
	//Post physics / other game logic.
	virtual void LateUpdate(float deltaTime) {};
	virtual void Serialize(Serializer& ser) {};
	virtual void Deserialize(Serializer& ser) {};
	virtual void ConnectComponents() {};
	virtual void LoadListeners() {}
	virtual void DeallocateBehavior()
	{
		this->~Behavior();
	}

private:
	std::string BehaviorName;

protected:
	void addListener(GameEventType type, unsigned int IDreq = 0);
};
