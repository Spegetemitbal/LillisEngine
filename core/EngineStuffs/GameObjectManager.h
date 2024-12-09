#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/GameObjPool.h"
#include "MemoryManagement/BehaviorPool.h"
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

	LilObj<GameObject> addObject();
	LilObj<GameObject> addObject(float x, float y);
	vector<GameObject*> getObjectsRaw() { return objects->poolDir; };
	int getObjActive() { return objects->GetActiveLine(); };

	LilObj<RectangleCollider> addCollider(float w, float h, int id);
	vector<RectangleCollider*> getCollidersRaw() { return colliderPool->poolDir; };
	int getColActive() { return colliderPool->GetActiveLine(); };
	//Do these really need to be pointers?? At least the built-in ones...

	LilObj<PlayerController> addPC();
	vector<PlayerController*> getPlayersRaw() { return playerPool->poolDir; };
	int getPlayerActive() { return playerPool->GetActiveLine(); };

	LilObj<Rotator> addRot(double angle);
	vector<Rotator*> getRotatorsRaw() { return rotatorPool->poolDir; };
	int getRotActive() { return rotatorPool->GetActiveLine(); };

	LilObj<Behavior> addBehavior(int* params);
	vector<Behavior*> getBehaviorsRaw() { return behaviors->poolDir;}
	//No getActive needed here, only active pointers are shown

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();

private:
	GameObjPool* objects;
	ComponentPool<Rotator>* rotatorPool;
	ComponentPool<PlayerController>* playerPool;
	ComponentPool<RectangleCollider>* colliderPool;
	BehaviorHandler* behaviors;
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.