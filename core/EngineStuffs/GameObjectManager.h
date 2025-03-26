#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/BehaviorPool.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "UI/UIObject.h"
#include "Tilemaps/TileGrid.h"
#include "Tilemaps/TileMap.h"

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

	LilObj<RigidBody> addRigidbody(RigidBodyShape shape, RigidBodyType rbtype, float mass, float density, PhysicsMaterial material,
		BoxData boxData, CircleData circleData);
	ActiveTracker<RigidBody*> getRBsRaw() const { return rigidBodyPool->getPool(); };
	unsigned int getRBActive() { return rigidBodyPool->GetActiveLine(); };
	void compactRigidBodies(int active)
	{rigidBodyPool->CompactPool(active);}

	LilObj<Sprite> addSprite(const std::string& name, unsigned int layer = 0);
	void setSpriteLayer(Sprite* spr);
	ActiveTracker<Sprite*> getSpritesRaw() const { return spritePool->getPool(); };
	unsigned int getSprActive() { return spritePool->GetActiveLine(); };
	void compactSprites(int active) {spritePool->CompactPool(active);}

	LilObj<Animator> addAnimator();
	ActiveTracker<Animator*> getAnimatorsRaw() const { return animatorPool->getPool(); };
	unsigned int getAnimActive() {return animatorPool->GetActiveLine(); };
	void compactAnimators(int active) {animatorPool->CompactPool(active);}

	LilObj<Behavior> addBehavior(const std::string &name) const;
	ActiveTracker<Behavior*> getBehaviorsRaw() const { return behaviors->getPool();}
	void compactBehaviors(int active) {behaviors->CompactPool(active);}
	//No getActive needed here, only active pointers are shown

	//A flyweight should be implemented here soon.
	//std::vector<Texture2D> sprites = std::vector<Texture2D>();
	TileMap* createTileMap(TileSet tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions);
	TileGrid* createTileGrid(GridShape tileShape, glm::vec2 tileSize);
	std::vector<TileMap>& getTileMaps() { return tileMaps; }
	TileGrid* getTileGrid() const {return worldGrid;};

private:

	TileGrid* worldGrid = nullptr;
	std::vector<TileMap> tileMaps;

	//For now these are tightly bound.
	GameObjPool* objects;
	ComponentPool<Transform>* transformPool;

	SceneGraph* sceneGraph;
	ComponentPool<Sprite>* spritePool;
	ComponentPool<Animator>* animatorPool;
	RenderOrder* renderOrder;
	ComponentPool<RectangleCollider>* colliderPool;
	ComponentPool<RigidBody>* rigidBodyPool;
	BehaviorHandler* behaviors;
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.