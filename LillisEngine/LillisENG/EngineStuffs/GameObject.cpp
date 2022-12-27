#include "GameObject.h"
#include "GameObjectManager.h"

#define WORLD GameObjectManager::world

RectangleRenderer* GameObject::CreateRenderer(Color c, float w, float h)
{
	if (renderer == nullptr)
	{
		renderer = &WORLD->renderers[WORLD->numRenderers];
		WORLD->numRenderers++;
		*renderer = RectangleRenderer(c, w, h);
		renderer->setControlledObject(this);
	}
	return renderer;
}

RectangleCollider* GameObject::CreateCollider(float w, float h)
{
	if (collider == nullptr)
	{
		collider = &WORLD->colliders[WORLD->numColliders];
		WORLD->numColliders++;
		*collider = RectangleCollider(w, h);
		collider->setControlledObject(this);
	}
	return collider;
}

PlayerController* GameObject::CreatePlayerController()
{
	int i = 0;
	if (player == nullptr)
	{
		player = &WORLD->players[WORLD->numPlayers];
		WORLD->numPlayers++;
		*player = PlayerController();
		player->setControlledObject(this);
	}
	return player;
}

Rotator* GameObject::CreateRotator(double angle)
{
	if (rotator == nullptr)
	{
		rotator = &WORLD->rotators[WORLD->numRotators];
		WORLD->numRotators++;
		*rotator = Rotator(angle);
		rotator->setControlledObject(this);
	}
	return rotator;
}