#include "pch.h"
#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{
	colliderPool = DBG_NEW LILLIS::ComponentPool<RectangleCollider>();
}

GameObject* GameObjectManager::addObject()
{
	objects.push_back(new GameObject());
	return objects.back();
	numObjects++;
}

GameObject* GameObjectManager::addObject(float x, float y)
{
	objects.push_back(new GameObject(x, y, numObjects));
	return objects.back();
	numObjects++;
}

void GameObjectManager::clearAll()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	for (int i = 0; i < colliders.size(); i++)
	{
		delete colliders[i];
	}

	for (int i = 0; i < players.size(); i++)
	{
		delete players[i];
	}

	for (int i = 0; i < rotators.size(); i++)
	{
		delete rotators[i];
	}

	objects.clear();
	colliders.clear();

	delete colliderPool;

	//sprites.clear();
	players.clear();
	rotators.clear();
}

GameObjectManager* GameObjectManager::world = nullptr;