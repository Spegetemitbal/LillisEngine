#pragma once

//Wipe these later.
#include "Behavior.h"
#include "../MemoryManagement/Serializer.h"

typedef Behavior*( *BehaviorFactory )(char* loc);
//typedef void( *BehaviorSerialize )( const Behavior* _Behavior, Serializer& o_Buffer );
//typedef void( *BehaviorDeserialize )( Behavior* _Behavior, Serializer& _Buffer );

struct BehaviorData
{
    BehaviorData()
    {
        generator = nullptr;
    }

    BehaviorData(BehaviorFactory factory, size_t bytes)
    {
        generator = factory;
        byteSize = bytes;
    }

    BehaviorFactory generator;
    size_t byteSize;
};

class BehaviorSystem
{
public:
    static void RegisterBehavior(const std::string& name, size_t bytes, BehaviorFactory factory);
    static BehaviorData GetBehavior(const std::string& name)
    {
        if (behaviorMap.contains(name))
        {
            return behaviorMap[name];
        } else
        {
            return {};
        }
    }
private:
    static std::unordered_map<std::string, BehaviorData> behaviorMap;
};