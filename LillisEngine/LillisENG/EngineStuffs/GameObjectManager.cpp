#include "GameObjectManager.h"

GameObject* GameObjectManager::addObject()
{
	if (numObjects < MAX_OBJECTS)
	{
		objects[numObjects] = GameObject();
		numObjects++;
		return &objects[numObjects];
	}
	return nullptr;
}

GameObjectManager* GameObjectManager::world = nullptr;