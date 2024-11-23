
//#include "ComponentPool.h"
//#include "Physics/RectangleCollider.h"

#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(10);
	colliderPool = DBG_NEW LILLIS::ComponentPool<RectangleCollider>(10);
	playerPool = DBG_NEW LILLIS::ComponentPool<PlayerController>(10);
	rotatorPool = DBG_NEW LILLIS::ComponentPool<Rotator>(10);
}

GameObjectManager::~GameObjectManager() 
{ 
	clearAll(); 
	delete objects;
	delete colliderPool;
	delete playerPool;
	delete rotatorPool;
}

GameObject* GameObjectManager::addObject()
{
	GameObject* g = objects->AddObject();
	g->transform.x = 0;
	g->transform.y = 0;
	g->transform.z = 0;
	numObjects++;
	return g;
}

GameObject* GameObjectManager::addObject(float x, float y)
{
	GameObject* g = objects->AddObject();
	g->transform.x = x;
	g->transform.y = y;
	g->transform.z = 0;
	numObjects++;
	return g;
}

RectangleCollider* GameObjectManager::addCollider(float w, float h, int id)
{
	RectangleCollider* r = colliderPool->AddComponent();
	r->setHeight(h);
	r->setWidth(w);
	r->setTag(id);
	return r;
}

PlayerController* GameObjectManager::addPC()
{
	PlayerController* p = playerPool->AddComponent();
	return p;
}

Rotator* GameObjectManager::addRot(double angle)
{
	Rotator* r = rotatorPool->AddComponent();
	r->setAngle(angle);
	return r;
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