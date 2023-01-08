#include "GameObject.h"

#define WORLD GameObjectManager::world

RectangleRenderer* GameObject::CreateRenderer(Color c, float w, float h)
{
	if (renderer == nullptr)
	{
		WORLD->renderers.push_back(RectangleRenderer(c, w, h));
		renderer = &WORLD->renderers.back();
		renderer->setControlledObject(this);
	}
	return renderer;
}

RectangleCollider* GameObject::CreateCollider(float w, float h)
{
	if (collider == nullptr)
	{
		WORLD->colliders.push_back(RectangleCollider(w, h));
		collider = &WORLD->colliders.back();
		collider->setControlledObject(this);
	}
	return collider;
}

PlayerController* GameObject::CreatePlayerController()
{
	if (player == nullptr)
	{
		WORLD->players.push_back(PlayerController());
		player = &WORLD->players.back();
		player->setControlledObject(this);
	}
	return player;
}

Rotator* GameObject::CreateRotator(double angle)
{
	if (rotator == nullptr)
	{
		WORLD->rotators.push_back(Rotator(angle));
		rotator = &WORLD->rotators.back();
		rotator->setControlledObject(this);
	}
	return rotator;
}