#pragma once

#include "MemoryManagement/ComponentPool.h"
#include "MemoryManagement/BehaviorPool.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "UI/UIObject.h"
#include "Tilemaps/TileGrid.h"
#include "Tilemaps/TileMap.h"
#include "Particles/ParticleEmitter.h"

class GameObject;
class GameObjPool;
class SceneGraph;
class RenderOrder;
class ObjectGrouping;
class BackgroundManager;

class GameWorld
{
public:

	void clearAll();

	GameWorld();
	
	~GameWorld();

	int numObjects = 0;

	//GameObject* findObject(GameObject g);

	LilObj<GameObject> addObject(const std::string &name);
	LilObj<GameObject> addObject(float x, float y, const std::string& name);
	bool SetTransformParent(const std::string& parent, LilObj<GameObject> child);
	void RemoveObjectParent(LilObj<GameObject> child, bool inactive = false);
	void RunTransformHierarchy(bool noFlagUpdate = false);
	LilObj<GameObject> getObjectByName(const std::string& name);
	ActiveTracker<GameObject*> getObjectsRaw() const;
	unsigned int getObjActive() const;
	void compactObjects(int active) const;

	LilObj<RigidBody> addRigidbody(int tag, RigidBodyShape shape, RigidBodyType rbtype,
	                               float mass, float density, PhysicsMaterial material,
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
	void cacheSingleAnim(unsigned int ID, Animation* anim)
	{
		singleAnimCache.emplace(ID, anim);
	}
	void cacheMultiAnim(unsigned int ID, StateObject* obj)
	{
		multiAnimCache.emplace(ID, obj);
	}

	LilObj<Behavior> addBehavior(const std::string &name) const;
	ActiveTracker<Behavior*> getBehaviorsRaw() const { return behaviors->getPool();}
	void compactBehaviors(int active) {behaviors->CompactPool(active);}
	void initializeAllComponents();
	//No getActive needed here, only active pointers are shown

	LilObj<ParticleEmitter> addParticleEmitter(ParticleEmitterData& data) const;
	ActiveTracker<ParticleEmitter*> getEmittersRaw() const { return emitterPool->getPool(); }
	void compactEmitters(int active) {emitterPool->CompactPool(active);}
	unsigned int getEmittersActive() {return emitterPool->GetActiveLine(); };

	TileMap* createTileMap(const TileSet& tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions, unsigned int layer);
	TileGrid* createTileGrid(GridShape tileShape, glm::vec2 tileSize);
	std::vector<TileMap>& getTileMaps() { return tileMaps; }
	TileGrid* getTileGrid() const {return worldGrid;};

	ObjectGrouping* const objectGrouping() const {return objectGroup;}
	BackgroundManager* const backgrounds() const {return backgroundManager;}

private:

	TileGrid* worldGrid = nullptr;
	std::vector<TileMap> tileMaps;

	std::unordered_map<unsigned int, Animation*> singleAnimCache = std::unordered_map<unsigned int, Animation*>();
	std::unordered_map<unsigned int, StateObject*> multiAnimCache = std::unordered_map<unsigned int, StateObject*>();

	//For now these are tightly bound.
	GameObjPool* objects;
	ComponentPool<Transform>* transformPool;

	BackgroundManager* backgroundManager;
	ObjectGrouping* objectGroup;
	SceneGraph* sceneGraph;
	ComponentPool<Sprite>* spritePool;
	ComponentPool<Animator>* animatorPool;
	RenderOrder* renderOrder;
	ComponentPool<RigidBody>* rigidBodyPool;
	BehaviorHandler* behaviors;
	ComponentPool<ParticleEmitter>* emitterPool;

};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.