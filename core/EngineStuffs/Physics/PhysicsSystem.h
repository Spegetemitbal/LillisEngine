#pragma once

#include <glm/vec2.hpp>

#include "RectangleCollider.h"
#include "RigidBody.h"

class PhysicsSystem
{
public:
    glm::vec2 getGravity() const { return gravity; };
    void PhysicsStep(float deltaTime, ActiveTracker<RigidBody*> &physObjects);
private:
    static bool CollideCheck(RigidBody bodyA, RigidBody bodyB, glm::vec2 &normal, float &depth);
    glm::vec2 gravity = glm::vec2(0.0f, -9.81f);
};
