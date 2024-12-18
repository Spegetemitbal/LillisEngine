#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/BehaviorPool.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"

class GameObject;
class GameObjPool;

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
	ActiveTracker<GameObject*> getObjectsRaw() const;
	unsigned int getObjActive() const;
	void compactObjects(int active) const;

	LilObj<RectangleCollider> addCollider(float w, float h, int id);
	ActiveTracker<RectangleCollider*> getCollidersRaw() const { return colliderPool->getPool(); };
	unsigned int getColActive() { return colliderPool->GetActiveLine(); };
	void compactColliders(int active) {colliderPool->CompactPool(active);}

	LilObj<PlayerController> addPC();

	LilObj<Rotator> addRot(double angle);

	LilObj<Behavior> addBehavior(const std::string &name) const;
	ActiveTracker<Behavior*> getBehaviorsRaw() const { return behaviors->getPool();}
	void compactBehaviors(int active) {behaviors->CompactPool(active);}
	//No getActive needed here, only active pointers are shown

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();

private:
	GameObjPool* objects;
	ComponentPool<RectangleCollider>* colliderPool;
	BehaviorHandler* behaviors;
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.