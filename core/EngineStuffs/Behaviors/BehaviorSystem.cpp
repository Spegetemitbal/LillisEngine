#include "BehaviorSystem.h"

BehaviorSystem* BehaviorSystem::behaviorSystem = nullptr;

void BehaviorSystem::InjectBehavior(std::string id, Behavior* behavior)
{
    behaviorMap.emplace(id, behavior);
}

void BehaviorSystem::PrintBehaviors()
{
    for (auto b : behaviorMap)
    {
        std::cout << b.second->GetName() << '\n';
    }
}

BehaviorSystem::BehaviorSystem()
{
    behaviorMap = unordered_map<std::string, Behavior*>();
}

BehaviorSystem::~BehaviorSystem()
{
    for (auto b : behaviorMap)
    {
        delete b.second;
        b.second = nullptr;
    }
}


//Singleton getter
BehaviorSystem* BehaviorSystem::getInstance()
{
    if (behaviorSystem != nullptr)
    {
        return behaviorSystem;
    }
    return nullptr;
}

//Singleton init
BehaviorSystem* BehaviorSystem::createInstance()
{
    if (behaviorSystem == nullptr)
    {
        behaviorSystem = DBG_NEW BehaviorSystem();
    }
    return behaviorSystem;
}

//Singleton delete
void BehaviorSystem::delInstance()
{
    delete behaviorSystem;
    behaviorSystem = nullptr;
}

