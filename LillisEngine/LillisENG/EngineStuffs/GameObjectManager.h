#pragma once

#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "ComponentPool.h"

class GameObject;

class GameObjectManager
{
public:
	static GameObjectManager* world;

	static void DelWorld() { delete world; }

	void clearAll();

	GameObjectManager();
	
	~GameObjectManager() { clearAll(); };

	int numObjects = 0;

	//GameObject* findObject(GameObject g);

	GameObject* addObject();
	GameObject* addObject(float x, float y);

	//Use pointers instead. BUT have each of these point to a contiguous stack of your own creation to make sure your cache is happy!.

	std::vector<GameObject*> objects = std::vector<GameObject*>();
	std::vector<RectangleCollider*> colliders = std::vector<RectangleCollider*>();

	LILLIS::ComponentPool<RectangleCollider>* colliderPool;

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();
	std::vector<PlayerController*> players = std::vector<PlayerController*>();
	std::vector<Rotator*> rotators = std::vector<Rotator*>();
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.