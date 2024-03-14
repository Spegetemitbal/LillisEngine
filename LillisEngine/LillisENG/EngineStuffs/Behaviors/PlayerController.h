#pragma once
#include "../Behaviors/Behavior.h"
#include "../Transform.h"

class GameObject;

class PlayerController : public Behavior
{
public:
	PlayerController() : Behavior("PlayerController")
	{
		addListener(INPUT_EVENT);
	}
	~PlayerController() {};

	static Component* CreatePlayerController(GameObject* G, int* param);
	void Update();
	void handleEvent(const Event& theEvent);
private:
	bool w = false, s = false, a = false, d = false;
};