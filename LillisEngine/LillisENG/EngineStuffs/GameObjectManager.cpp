#include "GameObjectManager.h"
#include "GameObject.h"

GameObject* GameObjectManager::addObject()
{
	objects.push_back(GameObject());
	return &objects.back();
}

void GameObjectManager::clearAll()
{
	objects.clear();
	colliders.clear();
	renderers.clear();
	players.clear();
	rotators.clear();
}

GameObjectManager* GameObjectManager::world = nullptr;