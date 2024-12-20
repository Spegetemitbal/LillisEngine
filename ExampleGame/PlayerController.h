#pragma once
#include "Lillis.h"

class GameObject;

class PlayerController : public Behavior
{
public:
	PlayerController() : Behavior("PlayerController")
	{
		addListener(INPUT_EVENT);
	}

	~PlayerController() {};

	static Behavior* CreatePlayerController(char* loc);
	void Update(float deltaTime) override;
	void handleEvent(const Event& theEvent);

	bool w = false, s = false, a = false, d = false;
private:
	
};