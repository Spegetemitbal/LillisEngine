#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/BehaviorPool.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"

class GameObject;
class GameObjPool;
class SceneGraph;
class RenderOrder;

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

	LilObj<GameObject> addObject(const std::string &name);
	LilObj<GameObject> addObject(float x, float y, const std::string& name);
	bool SetObjectParent(const std::string& parent, LilObj<GameObject> child);
	void RemoveObjectParent(LilObj<GameObject> child, bool inactive = false);
	void RunTransformHierarchy();
	LilObj<GameObject> getObjectByName(const std::string& name);
	ActiveTracker<GameObject*> getObjectsRaw() const;
	unsigned int getObjActive() const;
	void compactObjects(int active) const;

	LilObj<RectangleCollider> addCollider(float w, float h, int id);
	ActiveTracker<RectangleCollider*> getCollidersRaw() const { return colliderPool->getPool(); };
	unsigned int getColActive() { return colliderPool->GetActiveLine(); };
	void compactColliders(int active) {colliderPool->CompactPool(active);}

	LilObj<Sprite> addSprite(const std::string& name);
	void setSpriteLayer(Sprite* spr);
	ActiveTracker<Sprite*> getSpritesRaw() const { return spritePool->getPool(); };
	unsigned int getSprActive() { return spritePool->GetActiveLine(); };
	void compactSprites(int active) {spritePool->CompactPool(active);}

	LilObj<Behavior> addBehavior(const std::string &name) const;
	ActiveTracker<Behavior*> getBehaviorsRaw() const { return behaviors->getPool();}
	void compactBehaviors(int active) {behaviors->CompactPool(active);}
	//No getActive needed here, only active pointers are shown

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();

private:

	GameObjPool* objects;
	SceneGraph* sceneGraph;
	ComponentPool<Sprite>* spritePool;
	RenderOrder* renderOrder;
	ComponentPool<RectangleCollider>* colliderPool;
	BehaviorHandler* behaviors;
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.