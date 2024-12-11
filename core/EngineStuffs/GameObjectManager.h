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
	ActiveTracker<GameObject*> getObjectsRaw() { return objects->getPool(); };
	int getObjActive() { return objects->GetActiveLine(); };
	void compactObjects(int active) {objects->CompactPool(active);}

	LilObj<RectangleCollider> addCollider(float w, float h, int id);
	ActiveTracker<RectangleCollider*> getCollidersRaw() { return colliderPool->getPool(); };
	int getColActive() { return colliderPool->GetActiveLine(); };
	void compactColliders(int active) {colliderPool->CompactPool(active);}

	LilObj<PlayerController> addPC();
	ActiveTracker<PlayerController*> getPlayersRaw() { return playerPool->getPool(); };
	int getPlayerActive() { return playerPool->GetActiveLine(); };
	void compactPlayers(int active) {playerPool->CompactPool(active);}

	LilObj<Rotator> addRot(double angle);
	ActiveTracker<Rotator*> getRotatorsRaw() { return rotatorPool->getPool(); };
	int getRotActive() { return rotatorPool->GetActiveLine(); };
	void compactRotators(int active) {rotatorPool->CompactPool(active);}

	LilObj<Behavior> addBehavior(int* params);
	ActiveTracker<Behavior*> getBehaviorsRaw() { return behaviors->getPool();}
	void compactBehaviors(int active) {behaviors->CompactPool(active);}
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