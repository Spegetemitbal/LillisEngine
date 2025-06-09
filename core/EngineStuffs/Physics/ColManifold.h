//
// Created by Somed on 2/19/2025.
//

#ifndef COLMANIFOLD_H
#define COLMANIFOLD_H

#include "RigidBody.h"

class TileCollider;

struct ColManifold {
    //Perhaps change this for safety later.
    RigidBody* BodyA = nullptr;
    RigidBody* BodyB = nullptr;
    const TileCollider* Tile = nullptr;

    glm::vec2 Normal = {};
    float Depth = 0.0f;

    glm::vec2 Contact1 = {}, Contact2 = {};
    int ContactCount = 0;

    ColManifold(RigidBody* bodyA, RigidBody* bodyB, const TileCollider* tile, glm::vec2 normal, float depth,
        int numContacts, glm::vec2 contact1, glm::vec2 contact2 = {})
    {
        BodyA = bodyA;
        BodyB = bodyB;
        Normal = normal;
        Depth = depth;
        ContactCount = numContacts;
        Contact1 = contact1;
        Contact2 = contact2;
        Tile = tile;
    };

    ColManifold() = default;
    ~ColManifold() = default;
};



#endif //COLMANIFOLD_H
