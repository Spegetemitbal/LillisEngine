#pragma once

#include "GameObjectManager.h"
#include "Transform.h"

class GameObject
{
public:
	//Init all components to nullptr
	GameObject()
	{
		transform = Transform();
		renderer = nullptr;
		collider = nullptr;
		player = nullptr;
		rotator = nullptr;
	};

	~GameObject()
	{

	}

	RectangleRenderer* CreateRenderer(Color c, float w, float h);
	RectangleCollider* CreateCollider(float w, float h);
	PlayerController* CreatePlayerController();
	Rotator* CreateRotator(double angle);

	Transform transform;

	RectangleCollider* getCollider() { return collider; }
	RectangleRenderer* getRenderer() { return renderer; }

protected:
	RectangleRenderer* renderer = nullptr;
	RectangleCollider* collider = nullptr;
	PlayerController* player = nullptr;
	Rotator* rotator = nullptr;

};