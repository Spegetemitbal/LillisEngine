#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(10);
	colliderPool = DBG_NEW ComponentPool<RectangleCollider>(10);
	playerPool = DBG_NEW ComponentPool<PlayerController>(10);
	rotatorPool = DBG_NEW ComponentPool<Rotator>(10);
}

GameObjectManager::~GameObjectManager() 
{ 
	clearAll(); 
	delete objects;
	delete colliderPool;
	delete playerPool;
	delete rotatorPool;
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
	PlayerController* p = playerPool->AddComponent();
	p->LoadListeners();
	return {playerPool, p->GetID()};
}

LilObj<Rotator> GameObjectManager::addRot(double angle)
{
	Rotator* r = rotatorPool->AddComponent();
	r->setAngle(angle);
	return {rotatorPool, r->GetID()};
}

void GameObjectManager::clearAll()
{
	objects->ClearPool();
	playerPool->ClearPool();
	rotatorPool->ClearPool();
	colliderPool->ClearPool();
	numObjects = 0;
	//sprites.clear();
}

//GameObjectManager* GameObjectManager::world = nullptr;