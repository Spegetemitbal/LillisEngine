#pragma once

//Wipe these later.
#include "PlayerController.h"
#include "Rotator.h"
//#include "Behavior.h"

typedef Behavior*( *BehaviorFactory )(char* loc);
typedef void( *BehaviorSerialize )( const Behavior* _Behaviour/*, Buffer& o_Buffer*/ );
typedef void( *BehaviorDeserialize )( Behavior* _Behaviour/*, const Buffer& _Buffer*/ );

struct BehaviorData
{
    BehaviorData()
    {
        generator = nullptr;
        serializer = nullptr;
        deserializer = nullptr;
    }

    BehaviorData(BehaviorFactory factory, size_t bytes, BehaviorSerialize serialize = nullptr, BehaviorDeserialize deserialize = nullptr)
    {
        generator = factory;
        serializer = serialize;
        deserializer = deserialize;
        byteSize = bytes;
    }

    BehaviorFactory generator;
    BehaviorSerialize serializer;
    BehaviorDeserialize deserializer;
    size_t byteSize;
};

class BehaviorSystem
{
public:
    static void RegisterBehavior(std::string name, size_t bytes, BehaviorFactory factory, BehaviorSerialize serialize = nullptr, BehaviorDeserialize deserialize = nullptr);
    static BehaviorData GetBehavior(std::string name)
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