#include "GameObjectManager.h"
#include "GameObject.h"
#include "MemoryManagement/GameObjPool.h"
#include "SceneGraph.h"
#include "Graphics/RenderOrder.h"
#include "UI/UISystem.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(50);
	transformPool = DBG_NEW ComponentPool<Transform>(50);

	sceneGraph = DBG_NEW SceneGraph(transformPool);
	rigidBodyPool = DBG_NEW ComponentPool<RigidBody>(50);
	colliderPool = DBG_NEW ComponentPool<RectangleCollider>(50);
	spritePool = DBG_NEW ComponentPool<Sprite>(50);
	animatorPool = DBG_NEW ComponentPool<Animator>(50);
	renderOrder = DBG_NEW RenderOrder(spritePool);
	behaviors = DBG_NEW BehaviorHandler(50);
}

GameObjectManager::~GameObjectManager() 
{ 
	//clearAll();
	delete objects;
	delete transformPool;
	delete sceneGraph;
	delete spritePool;
	delete animatorPool;
	delete renderOrder;
	delete rigidBodyPool;
	delete colliderPool;
	delete behaviors;
}

LilObj<GameObject> GameObjectManager::addObject(const std::string& name)
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

LilObj<GameObject> GameObjectManager::addObject(float x, float y, const std::string& name)
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

LilObj<GameObject> GameObjectManager::getObjectByName(const std::string &name)
{
	return objects->GetObjectByName(name);
}


LilObj<RectangleCollider> GameObjectManager::addCollider(float w, float h, int id)
{
	RectangleCollider* r = colliderPool->AddComponent();
	r->setHeight(h);
	r->setWidth(w);
	r->setTag(id);
	return {colliderPool, r->GetID()};
}

LilObj<RigidBody> GameObjectManager::addRigidbody(RigidBodyShape shape, RigidBodyType rbType, float mass, float density, PhysicsMaterial material,
		BoxData boxData, CircleData circleData)
{
	RigidBody* r = rigidBodyPool->AddComponent();
	r->bodyShape = shape;
	r->bodyType = rbType;
	r->SetMass(mass);
	r->SetDensity(density);

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


LilObj<Sprite> GameObjectManager::addSprite(const std::string& name, unsigned int layer)
{
	//Probs refactor this later. Have component pools spit out smart pointers.
	Sprite* s = spritePool->AddComponent();
	LilObj<Sprite> sP = {spritePool, s->GetID()};
	sP->image = name;
	sP->SetLayer(layer);
	return sP;
}

void GameObjectManager::setSpriteLayer(Sprite* spr)
{
	renderOrder->MoveSprite({spritePool, spr->GetID()});
}

LilObj<Animator> GameObjectManager::addAnimator()
{
	Animator* a = animatorPool->AddComponent();
	return {animatorPool, a->GetID()};
}


void GameObjectManager::clearAll()
{
	if (numObjects > 0)
	{
		sceneGraph->ClearHierarchy();
		objects->ClearPool();
		transformPool->ClearPool();
		colliderPool->ClearPool();
		spritePool->ClearPool();
		animatorPool->ClearPool();
		behaviors->ClearPool();
		UISystem::getInstance()->clearUIObjects();
		numObjects = 0;
	}
	//sprites.clear();
}

LilObj<Behavior> GameObjectManager::addBehavior(const std::string &name) const
{
	return behaviors->CreateBehavior(name);
}

bool GameObjectManager::SetObjectParent(const std::string& parent, LilObj<GameObject> child)
{
	if (child->getRigidBody().Exists())
	{
		std::cout << "Physics objects cannot be children!" << std::endl;
		return false;
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

void GameObjectManager::RemoveObjectParent(LilObj<GameObject> child, bool inactive)
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

void GameObjectManager::RunTransformHierarchy()
{
	return sceneGraph->DoForwardKinematics();
}


ActiveTracker<GameObject*> GameObjectManager::getObjectsRaw() const { return objects->getPool(); };
unsigned int GameObjectManager::getObjActive() const { return objects->GetActiveLine(); };
void GameObjectManager::compactObjects(int active) const {objects->CompactPool(active);}

//GameObjectManager* GameObjectManager::world = nullptr;