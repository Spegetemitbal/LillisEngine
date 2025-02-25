//
// Created by Somed on 2/19/2025.
//

#ifndef COLMANIFOLD_H
#define COLMANIFOLD_H

#include "RigidBody.h"

struct ColManifold {
    //Perhaps change this for safety later.
    RigidBody* BodyA;
    RigidBody* BodyB;
    glm::vec2 Normal = {};
    float Depth;

    glm::vec2 Contact1 = {}, Contact2 = {};
    int ContactCount;

    ColManifold(RigidBody* bodyA, RigidBody* bodyB, glm::vec2 normal, float depth,
        int numContacts, glm::vec2 contact1, glm::vec2 contact2 = {})
    {
        BodyA = bodyA;
        BodyB = bodyB;
        Normal = normal;
        Depth = depth;
        ContactCount = numContacts;
        Contact1 = contact1;
        Contact2 = contact2;
    };
};



#endif //COLMANIFOLD_H
