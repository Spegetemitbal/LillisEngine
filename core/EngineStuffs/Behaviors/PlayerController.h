#pragma once
#include "Behavior.h"
#include "../Transform.h"

class GameObject;

class PlayerController : public Behavior
{
public:
	PlayerController() : Behavior("PlayerController")
	{}

	~PlayerController() {};

	void LoadListeners() override
	{
		addListener(INPUT_EVENT);
	}

	void Update(float deltaTime) override;
	void handleEvent(const Event& theEvent);

	bool w = false, s = false, a = false, d = false;
private:
	
};