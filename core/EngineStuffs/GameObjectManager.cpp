#include "GameObjectManager.h"
#include "GameObject.h"
#include "MemoryManagement/GameObjPool.h"
#include "SceneGraph.h"
#include "Graphics/RenderOrder.h"

GameObjectManager::GameObjectManager()
{
	objects = DBG_NEW GameObjPool(50);
	sceneGraph = DBG_NEW SceneGraph(objects);
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
	g->transform.SetLocalPosition({ 0, 0, 0 });
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
		colliderPool->ClearPool();
		spritePool->ClearPool();
		animatorPool->ClearPool();
		behaviors->ClearPool();
		renderOrder->Clear();
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

std::unordered_set<unsigned int> GameObjectManager::RunTransformHierarchy()
{
	return sceneGraph->DoForwardKinematics();
}

void GameObjectManager::doRenderOrder(const std::unordered_set<unsigned int>& toUpdate)
{
	renderOrder->OrderByAxis(toUpdate);
}

void GameObjectManager::initRenderOrder()
{
	renderOrder->OrderAll();
}


ActiveTracker<GameObject*> GameObjectManager::getObjectsRaw() const { return objects->getPool(); };
unsigned int GameObjectManager::getObjActive() const { return objects->GetActiveLine(); };
void GameObjectManager::compactObjects(int active) const {objects->CompactPool(active);}

//GameObjectManager* GameObjectManager::world = nullptr;