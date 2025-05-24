//
// Created by Somed on 4/5/2025.
//

#include "WorldManager.h"
#include "Utils/SceneLoader.h"
#include "Physics/PhysicsSystem.h"

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

void WorldManager::SetWorld()
{
    if (currentWorldName != nextLevel)
    {
        currentWorldName = nextLevel;
        current_world = loadedWorlds[nextLevel];
    }

    if (generateWorldData)
    {
        generateWorldData = false;
        current_world->clearAll();
        //return true;
        LoadWorld();
    }
    //return false;
}

void WorldManager::LoadWorld()
{
    SceneLoader::LoadData(currentWorldName);
    current_world->RunTransformHierarchy(true);
    ActiveTracker<RigidBody*> rb = current_world->getRBsRaw();
    unsigned int numRB = current_world->getRBActive();
    PhysicsSystem::getInstance()->InitRigidBodies(rb, numRB);
}


WorldManager *WorldManager::getInstance()
{
    if (world_manager == nullptr)
    {
        return createInstance();
    }
    return world_manager;
}

void WorldManager::destroyInstance()
{
    if (world_manager != nullptr)
    {
        delete world_manager;
        world_manager = nullptr;
    }
}



