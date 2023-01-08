#pragma once
#include "../Component.h"
#include "../Transform.h"
#include <SDL.h>

class GameObject;

class PlayerController : public Component
{
public:
	static Component* CreatePlayerController(GameObject* G, int* param);
	void Update(const Uint8* keyDown);
private:
};