//
// Created by Somed on 4/5/2025.
//

#include "WorldManager.h"

WorldManager::~WorldManager()
{
    DelAllWorlds();
}

WorldManager *WorldManager::createInstance()
{
    if (world_manager == nullptr)
    {
        world_manager = new WorldManager();
    }
    return world_manager;
}

WorldManager *WorldManager::getInstance()
{
    if (world_manager != nullptr)
    {
        return world_manager;
    }
    return nullptr;
}

void WorldManager::destroyInstance()
{
    if (world_manager != nullptr)
    {
        delete world_manager;
        world_manager = nullptr;
    }
}



