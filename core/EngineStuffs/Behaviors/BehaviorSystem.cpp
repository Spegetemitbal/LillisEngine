#include "BehaviorSystem.h"

std::unordered_map<std::string, BehaviorData> BehaviorSystem::behaviorMap = std::unordered_map<std::string, BehaviorData>();

void BehaviorSystem::RegisterBehavior(std::string name, size_t bytes, BehaviorFactory factory, BehaviorSerialize serialize, BehaviorDeserialize deserialize)
{
    if (behaviorMap.find(name) == behaviorMap.end())
    {
        if (serialize != nullptr && deserialize != nullptr)
        {
            behaviorMap[name] = BehaviorData(factory, bytes, serialize, deserialize);
        } else if (serialize != nullptr || deserialize != nullptr)
        {
            std::cout << "Both serializers and deserializers must be defined if not null";
        } else
        {
            behaviorMap[name] = BehaviorData(factory, bytes);
        }
    }
}
