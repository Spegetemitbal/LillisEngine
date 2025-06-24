//
// Created by Somed on 5/22/2025.
//

#ifndef PHYSICSEVENTHANDLER_H
#define PHYSICSEVENTHANDLER_H
#include <unordered_set>

#include "ColManifold.h"

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
        if (!collidedThisFrame.contains(identifiers))
        {
            collidedThisFrame.emplace(identifiers,cm);
        } else
        {
            collidedThisFrame[identifiers] = cm;
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
    std::unordered_map<std::pair<RigidBody*, RigidBody*>,ColManifold, IDHolder> collidedThisFrame;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> collidedLastFrame;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> colliderCache;
    std::vector<std::pair<RigidBody*, RigidBody*>> toRemove;
};



#endif //PHYSICSEVENTHANDLER_H
