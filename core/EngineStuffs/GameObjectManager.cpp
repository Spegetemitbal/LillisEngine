#include "GameObjectManager.h"
#include "GameObject.h"
#include "MemoryManagement/GameObjPool.h"
#include "SceneGraph.h"
#include "Graphics/RenderOrder.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(10);
	sceneGraph = DBG_NEW SceneGraph(objects);
	colliderPool = DBG_NEW ComponentPool<RectangleCollider>(10);
	spritePool = DBG_NEW ComponentPool<Sprite>(10);
	animatorPool = DBG_NEW ComponentPool<Animator>(10);
	renderOrder = DBG_NEW RenderOrder(spritePool);
	behaviors = DBG_NEW BehaviorHandler(50);
}

GameObjectManager::~GameObjectManager() 
{ 
	//clearAll();
	delete objects;
	delete sceneGraph;
	delete spritePool;
	delete animatorPool;
	delete renderOrder;
	delete colliderPool;
	delete behaviors;
}

LilObj<GameObject> GameObjectManager::addObject(const std::string& name)
{
	GameObject* g = objects->AddObject(0, 0, name);
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
	g->transform.localPosition.x = 0;
	g->transform.localPosition.y = 0;
	g->transform.localPosition.z = 0;
	numObjects++;
	return ob;
}

LilObj<GameObject> GameObjectManager::addObject(float x, float y, const std::string& name)
{
	GameObject* g = objects->AddObject(x, y, name);
	LilObj<GameObject> ob = {objects, g->GetID()};
	g->thisObject = ob;
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

LilObj<Sprite> GameObjectManager::addSprite(const std::string& name)
{
	Sprite* s = spritePool->AddComponent();
	s->image = name;
	return {spritePool, s->GetID()};
}

void GameObjectManager::setSpriteLayer(Sprite* spr)
{
	renderOrder->MoveSprite({spritePool, spr->GetID()});
}

LilObj<Animator> GameObjectManager::addSingleAnimator(Animation *anim)
{
	Animator* a = animatorPool->AddComponent();
	a->SetSingleAnimation(anim);
	return {animatorPool, a->GetID()};
}


LilObj<Animator> GameObjectManager::addAnimator(StateObject* stateObj)
{
	Animator* a = animatorPool->AddComponent();
	a->SetMultiAnimation(stateObj);
	return {animatorPool, a->GetID()};
}


void GameObjectManager::clearAll()
{
	if (numObjects > 0)
	{
		sceneGraph->ClearHierarchy();
		objects->ClearPool();
		colliderPool->ClearPool();
		spritePool->ClearPool();
		animatorPool->ClearPool();
		behaviors->ClearPool();
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
	if (child.Exists())
	{
		LilObj<GameObject> g = getObjectByName(parent);
		if (g.Exists())
		{
			sceneGraph->SetParent(g, child);
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
			sceneGraph->RemoveParent(child, ObjectRemovalFlag::OBJECTREMOVAL_DESTROY);
		}
		sceneGraph->RemoveParent(child, ObjectRemovalFlag::OBJECTREMOVAL_NONE);
	}
}

void GameObjectManager::RunTransformHierarchy()
{
	sceneGraph->DoForwardKinematics();
}




ActiveTracker<GameObject*> GameObjectManager::getObjectsRaw() const { return objects->getPool(); };
unsigned int GameObjectManager::getObjActive() const { return objects->GetActiveLine(); };
void GameObjectManager::compactObjects(int active) const {objects->CompactPool(active);}

//GameObjectManager* GameObjectManager::world = nullptr;