#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/GameObjPool.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"

class GameObject;

class GameObjectManager
{
public:
	static inline GameObjectManager* world;

	static void DelWorld() { delete world; }

	void clearAll();

	GameObjectManager();
	
	~GameObjectManager();

	int numObjects = 0;

	//GameObject* findObject(GameObject g);

	GameObject* addObject();
	GameObject* addObject(float x, float y);
	vector<GameObject*> getObjects() { return objects->poolDir; };
	int getObjActive() { return objects->GetActiveLine(); };

	RectangleCollider* addCollider(float w, float h, int id);
	vector<RectangleCollider*> getColliders() { return colliderPool->poolDir; };
	int getColActive() { return colliderPool->GetActiveLine(); };
	//Do these really need to be pointers?? At least the built-in ones...

	PlayerController* addPC();
	vector<PlayerController*> getPlayers() { return playerPool->poolDir; };
	int getPlayerActive() { return playerPool->GetActiveLine(); };

	Rotator* addRot(double angle);
	vector<Rotator*> getRotators() { return rotatorPool->poolDir; };
	int getRotActive() { return rotatorPool->GetActiveLine(); };

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();

private:
	GameObjPool* objects;
	ComponentPool<Rotator>* rotatorPool;
	ComponentPool<PlayerController>* playerPool;
	ComponentPool<RectangleCollider>* colliderPool;
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.