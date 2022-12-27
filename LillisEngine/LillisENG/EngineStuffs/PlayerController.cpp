#include "GameObject.h"

Component* PlayerController::CreatePlayerController(GameObject* G, int* params)
{
	return G->CreatePlayerController();
}

void PlayerController::Update(const Uint8* keyDown)
{

	int speed = 5;

	Transform* t = &thisObject->transform;
	//take in array, check against SDLK_(whatever), that should work

	if (keyDown[SDL_SCANCODE_W])
	{
		t->y -= speed;
	}
	else if (keyDown[SDL_SCANCODE_S])
	{
		t->y += speed;
	}
	if (keyDown[SDL_SCANCODE_A])
	{
		t->x -= speed;
	}
	else if (keyDown[SDL_SCANCODE_D])
	{
		t->x += speed;
	}
}