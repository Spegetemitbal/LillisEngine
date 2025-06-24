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

        std::pair<unsigned int, unsigned int> col = std::make_pair(identifiers.first->GetID(), identifiers.second->GetID());

        if (!collidedThisFrame.contains(col))
        {
            collidedThisFrame.emplace(col,cm);
        } else
        {
            collidedThisFrame[col] = cm;
        }
    }
    void IsColliding(RigidBody* rb, ColManifold cm)
    {
        if (cm.Tile == nullptr)
        {
            return;
        }
        std::pair<unsigned int, unsigned int> col = std::make_pair(rb->GetID(), rb->GetID());
        int tileTag = cm.Tile->collisionTag;

        if (!collidedThisFrame.contains(col))
        {
            collidedThisFrame.emplace(col,cm);
            tileCollisions.try_emplace(col.first, tileTag);
        } else
        {
            collidedThisFrame[col] = cm;
            //May need additional safety here.
            tileCollisions[col.first] = tileTag;
        }
    }

    void ClearMatrix()
    {
        collidedThisFrame.clear();
        collidedLastFrame.clear();
    }
    bool GetIsColliding(const std::pair<RigidBody*, RigidBody*>& identifiers)
    {
        return collidedThisFrame.contains(std::make_pair(identifiers.first->GetID(), identifiers.second->GetID()));
    }
private:
    std::unordered_map<std::pair<unsigned int, unsigned int>,ColManifold, IDHolder> collidedThisFrame;
    std::unordered_set<std::pair<unsigned int, unsigned int>, IDHolder> collidedLastFrame;
    std::unordered_set<std::pair<unsigned int, unsigned int>, IDHolder> colliderCache;
    std::unordered_map<unsigned int, int> tileCollisions;
    std::vector<std::pair<unsigned int, unsigned int>> toRemove;
};



#endif //PHYSICSEVENTHANDLER_H
