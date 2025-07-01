#include "GameWorld.h"
#include "GameObject.h"
#include "MemoryManagement/GameObjPool.h"
#include "SceneGraph.h"
#include "Graphics/RenderOrder.h"
#include "UI/UISystem.h"
#include "ObjectGrouping.h"
#include "Particles/ParticleEmitter.h"
#include "Graphics/BackgroundManager.h"

GameWorld::GameWorld()
{
	objects = DBG_NEW GameObjPool(50);
	transformPool = DBG_NEW ComponentPool<Transform>(50);
	objectGroup = DBG_NEW ObjectGrouping();
	sceneGraph = DBG_NEW SceneGraph(transformPool);
	rigidBodyPool = DBG_NEW ComponentPool<RigidBody>(50);
	spritePool = DBG_NEW ComponentPool<Sprite>(50);
	animatorPool = DBG_NEW ComponentPool<Animator>(30);
	renderOrder = DBG_NEW RenderOrder(spritePool);
	behaviors = DBG_NEW BehaviorHandler(40);
	emitterPool = DBG_NEW ComponentPool<ParticleEmitter>(10);
	backgroundManager = DBG_NEW BackgroundManager();
}

GameWorld::~GameWorld() 
{ 
	//clearAll();
	delete objects;
	delete transformPool;
	delete sceneGraph;
	delete spritePool;
	delete animatorPool;
	delete renderOrder;
	delete rigidBodyPool;
	delete behaviors;
	delete emitterPool;
	delete objectGroup;
	delete backgroundManager;
	if (worldGrid != nullptr)
	{
		delete worldGrid;
	}
}

LilObj<GameObject> GameWorld::addObject(const std::string& name)
{
	GameObject* g = objects->AddObject(name);
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
	Transform* t = transformPool->AddComponent();
	t->SetLocalPosition({ 0, 0 });
	t->setControlledObject(ob);
	g->transform = {transformPool,t->GetID()};
	numObjects++;
	return ob;
}

LilObj<GameObject> GameWorld::addObject(float x, float y, const std::string& name)
{
	GameObject* g = objects->AddObject(name);
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
	Transform* t = transformPool->AddComponent();
	t->SetLocalPosition({ x, y });
	t->setControlledObject(ob);
	g->transform = {transformPool,t->GetID()};
	numObjects++;
	return ob;
}

LilObj<GameObject> GameWorld::getObjectByName(const std::string &name)
{
	return objects->GetObjectByName(name);
}

LilObj<RigidBody> GameWorld::addRigidbody(int tag, RigidBodyShape shape, RigidBodyType rbType, float mass, float density, PhysicsMaterial material,
                                          BoxData boxData, CircleData circleData)
{
	RigidBody* r = rigidBodyPool->AddComponent();
	r->bodyShape = shape;
	r->bodyType = rbType;
	r->SetMass(mass);
	r->SetDensity(density);
	r->SetColTag(tag);

	//Physics material stuff
	r->SetRestitution(material.restitution);
	r->SetDynamicFriction(material.dynamicFriction);
	r->SetStaticFriction(material.staticFriction);

	//Specifics
	if (shape == RigidBodyShape::RB_BOX)
	{
		r->SetSize(boxData.boxSize);
	}
	else if (shape == RigidBodyShape::RB_CIRCLE)
	{
		r->SetRadius(circleData.radius);
	} else
	{
		//Gotta do something here.
		throw;
	}
	r->InitVertices();

	return {rigidBodyPool, r->GetID()};
}


LilObj<Sprite> GameWorld::addSprite(const std::string& name, unsigned int layer)
{
	//Probs refactor this later. Have component pools spit out smart pointers.
	Sprite* s = spritePool->AddComponent();
	LilObj<Sprite> sP = {spritePool, s->GetID()};
	sP->image = name;
	sP->SetLayer(layer);
	return sP;
}

void GameWorld::setSpriteLayer(Sprite* spr)
{
	renderOrder->MoveSprite({spritePool, spr->GetID()});
}

LilObj<Animator> GameWorld::addAnimator()
{
	Animator* a = animatorPool->AddComponent();
	a->SetSelfID({animatorPool, a->GetID()});
	return {animatorPool, a->GetID()};
}


void GameWorld::clearAll()
{
	if (numObjects > 0)
	{
		sceneGraph->ClearHierarchy();
		objectGrouping()->ClearHierarchy();
		objects->ClearPool();
		transformPool->ClearPool();
		spritePool->ClearPool();
		animatorPool->ClearPool();
		behaviors->ClearPool();
		emitterPool->ClearPool();
		rigidBodyPool->ClearPool();
		UISystem::getInstance()->clearUIObjects();
		backgroundManager->ClearBackgrounds();
		numObjects = 0;
	}
	if (worldGrid != nullptr)
	{
		delete worldGrid;
		worldGrid = nullptr;
		tileMaps.clear();
	}
	//sprites.clear();
}

LilObj<Behavior> GameWorld::addBehavior(const std::string &name) const
{
	return behaviors->CreateBehavior(name);
}

void GameWorld::initializeAllComponents()
{
	auto b = behaviors->getPool();
	for (int i = 0; i < b.size(); i++)
	{
		b[i]->ConnectComponents();
		b[i]->LoadListeners();
	}
	auto anims = animatorPool->getPool();
	unsigned int numAnims = animatorPool->GetActiveLine();
	for (int i = 0; i < numAnims; i++)
	{
		anims[i]->ConnectComponents();
		unsigned int id = anims[i]->GetID();
		if (multiAnimCache.contains(id))
		{
			anims[i]->SetMultiAnimation(multiAnimCache[id]);
		} else if (singleAnimCache.contains(id))
		{
			anims[i]->SetSingleAnimation(singleAnimCache[id]);
		}
	}
	multiAnimCache.clear();
	singleAnimCache.clear();
}

LilObj<ParticleEmitter> GameWorld::addParticleEmitter(ParticleEmitterData &data) const
{
	ParticleEmitter* pe = emitterPool->AddComponent();
	pe->startPos = data.startPos;
	pe->spawnBounds = data.spawnBounds;
	pe->minInitVelocity = data.minInitVelocity;
	pe->maxInitVelocity = data.maxInitVelocity;
	pe->startAcceleration = data.startAcceleration;
	pe->minStartScale = data.minStartScale;
	pe->maxStartScale = data.maxStartScale;
	pe->startColor = data.startColor;
	pe->endColor = data.endColor;
	pe->lifetime = data.lifetime;
	pe->spawnSpeed = data.spawnSpeed;
	return {emitterPool, pe->GetID()};
}



bool GameWorld::SetTransformParent(const std::string& parent, LilObj<GameObject> child)
{
	if (child->getRigidBody().Exists())
	{
		if (!child->getRigidBody()->IsTrigger())
		{
			std::cout << "Non Trigger physics objects cannot be children!" << std::endl;
			return false;
		}
	}

	if (child.Exists())
	{
		LilObj<GameObject> g = getObjectByName(parent);
		if (g.Exists())
		{
			sceneGraph->SetParent(g->transform, child->transform);
			return true;
		} else
		{
			std::cout << "Cannot find parent object" << '\n';
			return false;
		}
	} else
	{
		std::cout << "Child doesn't exist" << '\n';
		return false;
	}
}

void GameWorld::RemoveObjectParent(LilObj<GameObject> child, bool inactive)
{
	if (child.Exists())
	{
		if (inactive)
		{
			sceneGraph->RemoveParent(child->transform, ObjectRemovalFlag::OBJECTREMOVAL_DESTROY);
		}
		sceneGraph->RemoveParent(child->transform, ObjectRemovalFlag::OBJECTREMOVAL_NONE);
	}
}

void GameWorld::RunTransformHierarchy(bool noFlagUpdate)
{
	return sceneGraph->DoForwardKinematics(noFlagUpdate);
}


ActiveTracker<GameObject*> GameWorld::getObjectsRaw() const { return objects->getPool(); };
unsigned int GameWorld::getObjActive() const { return objects->GetActiveLine(); };
void GameWorld::compactObjects(int active) const {objects->CompactPool(active);}

TileGrid *GameWorld::createTileGrid(GridShape tileShape, glm::vec2 tileSize)
{
	if (tileSize.x <= 0)
	{
		tileSize.x = 1;
	}
	if (tileSize.y <= 0)
	{
		tileSize.y = 1;
	}

	TileGrid* tg = DBG_NEW TileGrid(tileShape, tileSize);

	if (worldGrid != nullptr)
	{
		delete worldGrid;
		for (auto tm : tileMaps)
		{
			tm.setTileGrid(tg);
		}
	}

	worldGrid = tg;
	return worldGrid;
}

TileMap *GameWorld::createTileMap(const TileSet& tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions)
{
	if (dimensions.first < 5)
	{
		dimensions.first = 5;
	}
	if (dimensions.second < 5)
	{
		dimensions.second = 5;
	}

	if (worldGrid == nullptr)
	{
		std::cerr << "No world grid found!" << std::endl;
		return nullptr;
	}

	tileMaps.emplace_back(worldGrid, tileSet, gridIndex, dimensions);
	return &tileMaps.back();
}

//GameObjectManager* GameObjectManager::world = nullptr;