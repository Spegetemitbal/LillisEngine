//
// Created by Somed on 4/5/2025.
//

#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include "GameObjectManager.h"
#include "Audio/AudioSystem.h"

//The true SceneManager

class WorldManager {
public:

    static WorldManager* createInstance();

    static WorldManager* getInstance();

    static void destroyInstance();

    bool WorldIsLoaded(const std::string& name)
    {
        return loadedWorlds.contains(name);
    }

    void SetCurrentWorld(const std::string& name)
    {
        if (loadedWorlds.contains(name))
        {
            nextLevel = name;
        }
    }

    void SetAndReloadWorld(const std::string& name)
    {
        if (loadedWorlds.contains(name))
        {
            if (name == currentWorldName)
            {
                ReloadCurrentWorld();
                return;
            }

            nextLevel = name;
            loadedWorlds[name]->clearAll();
            generateWorldData = true;
        }
    }

    void DelWorld(const std::string& name)
    {
        if (loadedWorlds.contains(name))
        {
            GameObjectManager* delworld = loadedWorlds[name];
            delete delworld;
            if (delworld == current_world)
            {
                current_world = nullptr;
            }
            loadedWorlds.erase(name);
        }
    }

    //Makes a new world with the given name. If that world already exists, clears it instead.
    void MakeWorld(const std::string& name)
    {
        if (loadedWorlds.contains(name))
        {
            loadedWorlds[name]->clearAll();
        }
        auto* newWorld = DBG_NEW GameObjectManager();
        loadedWorlds.emplace(name, newWorld);

        if (current_world == nullptr)
        {
            current_world = newWorld;
            currentWorldName = name;
            nextLevel = name;
        }
    }

    void ReloadCurrentWorld()
    {
        if (current_world != nullptr)
        {
            current_world->clearAll();
            generateWorldData = true;
        }
    }

    void DelAllWorlds()
    {
        current_world = nullptr;
        for (const auto& world : loadedWorlds)
        {
            delete world.second;
        }
        loadedWorlds.clear();
    }

    GameObjectManager* GetCurrentWorld()
    {
        return current_world;
    }

    std::string GetCurrentWorldName() const
    {
        return currentWorldName;
    }

    static inline WorldManager* world_manager = nullptr;

private:

    friend class Engine;

    bool SetWorld()
    {
        if (currentWorldName != nextLevel)
        {
            currentWorldName = nextLevel;
            current_world = loadedWorlds[nextLevel];
        }

        if (generateWorldData)
        {
            generateWorldData = false;
            return true;
        }
        return false;
    }

    std::string nextLevel;
    bool generateWorldData = true;

    //TODO Make this a singleton
    WorldManager() = default;
    ~WorldManager();

    std::string currentWorldName;
    GameObjectManager* current_world = nullptr;
    std::map<std::string, GameObjectManager*> loadedWorlds;
};



#endif //WORLDMANAGER_H
