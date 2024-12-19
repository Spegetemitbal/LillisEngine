#include "BehaviorSystem.h"

std::unordered_map<std::string, BehaviorData> BehaviorSystem::behaviorMap = std::unordered_map<std::string, BehaviorData>();

void BehaviorSystem::RegisterBehavior(const std::string& name, size_t bytes, BehaviorFactory factory)
{
    if (behaviorMap.find(name) == behaviorMap.end())
    {
        behaviorMap[name] = BehaviorData(factory, bytes);
    }
}
