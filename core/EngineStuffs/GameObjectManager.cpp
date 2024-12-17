#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(10);
	colliderPool = DBG_NEW ComponentPool<RectangleCollider>(10);
	behaviors = DBG_NEW BehaviorHandler(50);
}

GameObjectManager::~GameObjectManager() 
{ 
	//clearAll();
	delete objects;
	delete colliderPool;
	delete behaviors;
}

LilObj<GameObject> GameObjectManager::addObject()
{
	GameObject* g = objects->AddObject();
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
	g->transform.x = 0;
	g->transform.y = 0;
	g->transform.z = 0;
	numObjects++;
	return ob;
}

LilObj<GameObject> GameObjectManager::addObject(float x, float y)
{
	GameObject* g = objects->AddObject();
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
	g->transform.x = x;
	g->transform.y = y;
	g->transform.z = 0;
	numObjects++;
	return ob;
}

LilObj<RectangleCollider> GameObjectManager::addCollider(float w, float h, int id)
{
	RectangleCollider* r = colliderPool->AddComponent();
	r->setHeight(h);
	r->setWidth(w);
	r->setTag(id);
	return {colliderPool, r->GetID()};
}

LilObj<PlayerController> GameObjectManager::addPC()
{
	LilObj<Behavior> b = behaviors->CreateBehavior("PlayerController");
	LilObj<PlayerController> pc = {behaviors, b->GetID()};
	//PlayerController* p = playerPool->AddComponent();
	return pc;
}

LilObj<Rotator> GameObjectManager::addRot(double angle)
{
	LilObj<Behavior> b = behaviors->CreateBehavior("Rotator");
	LilObj<Rotator> r = {behaviors, b.GetID()};
	//Rotator* r = rotatorPool->AddComponent();
	r->setAngle(angle);
	return r;
}

void GameObjectManager::clearAll()
{
	objects->ClearPool();
	colliderPool->ClearPool();
	behaviors->ClearPool();
	numObjects = 0;
	//sprites.clear();
}

//GameObjectManager* GameObjectManager::world = nullptr;