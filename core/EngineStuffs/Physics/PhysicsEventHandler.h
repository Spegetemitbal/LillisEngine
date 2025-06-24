//
// Created by Somed on 5/22/2025.
//

#ifndef PHYSICSEVENTHANDLER_H
#define PHYSICSEVENTHANDLER_H
#include <unordered_set>

#include "ColManifold.h"
#include "EngineStuffs/Tilemaps/TileCollider.h"

struct IDHolder
{
    template <class T1, class T2>
    size_t operator() (const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

class PhysicsEventHandler {
public:

    void TickFireEvent(MemoryPool* memPool);

    void IsColliding(std::pair<RigidBody*, RigidBody*> identifiers, ColManifold cm)
    {
        if (identifiers.first == identifiers.second)
        {
            return;
        }

        if (!collidedThisFrame.contains(identifiers))
        {
            collidedThisFrame.emplace(identifiers,cm);
        } else
        {
            collidedThisFrame[identifiers] = cm;
        }
    }
    void IsColliding(RigidBody* rb, ColManifold cm)
    {
        if (cm.Tile == nullptr)
        {
            return;
        }
        std::pair<RigidBody*,RigidBody*> pr = std::make_pair(rb,rb);
        int tileTag = cm.Tile->collisionTag;

        if (!collidedThisFrame.contains(pr))
        {
            collidedThisFrame.emplace(pr,cm);
            tileCollisions.try_emplace(rb, tileTag);
        } else
        {
            collidedThisFrame[pr] = cm;
            //May need additional safety here.
            tileCollisions[rb] = tileTag;
        }
    }

    void ClearMatrix()
    {
        collidedThisFrame.clear();
        collidedLastFrame.clear();
    }
    bool GetIsColliding(const std::pair<RigidBody*, RigidBody*>& identifiers)
    {
        return collidedThisFrame.contains(identifiers);
    }
private:
    //TODO add memory safety for persistent collisions
    std::unordered_map<std::pair<RigidBody*, RigidBody*>,ColManifold, IDHolder> collidedThisFrame;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> collidedLastFrame;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> colliderCache;
    std::unordered_map<RigidBody*, int> tileCollisions;
    std::vector<std::pair<RigidBody*, RigidBody*>> toRemove;
};



#endif //PHYSICSEVENTHANDLER_H
