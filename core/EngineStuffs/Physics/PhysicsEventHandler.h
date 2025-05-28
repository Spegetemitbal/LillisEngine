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
        if (!collisionMatrix.contains(identifiers))
        {
            collisionMatrix.emplace(identifiers,cm);
        } else
        {
            collisionMatrix[identifiers] = cm;
        }
    }
    void NotColliding(const std::pair<RigidBody*, RigidBody*>& identifiers)
    {
        if (collisionMatrix.contains(identifiers))
        {
            collisionMatrix.erase(identifiers);
            if (persistingCollisions.contains(identifiers))
            {
                persistingCollisions.erase(identifiers);
            }
            if (!leftCollision.contains(identifiers))
            {
                leftCollision.emplace(identifiers);
            }
        }
    }
    void ClearMatrix()
    {
        collisionMatrix.clear();
    }
    bool GetIsColliding(const std::pair<RigidBody*, RigidBody*>& identifiers)
    {
        return collisionMatrix.contains(identifiers);
    }
private:
    std::unordered_map<std::pair<RigidBody*, RigidBody*>,ColManifold, IDHolder> collisionMatrix;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> persistingCollisions;
    std::unordered_set<std::pair<RigidBody*, RigidBody*>, IDHolder> leftCollision;
};



#endif //PHYSICSEVENTHANDLER_H
