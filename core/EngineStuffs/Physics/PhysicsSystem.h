#pragma once

#include <glm/vec2.hpp>

#include "RectangleCollider.h"
#include "RigidBody.h"
#include "ColManifold.h"

struct PhysicsSettings
{
    unsigned int physicsIterations = 2;
    glm::vec2 gravity = glm::vec2(0.0f, -98.1f);
};

class PhysicsSystem
{
public:

    PhysicsSystem() = delete;
    static PhysicsSystem* createInstance(PhysicsSettings settings = PhysicsSettings());
    static PhysicsSystem* getInstance();
    static void destroyInstance();

    //TODO: Add this to engine settings
    static inline unsigned int MinIterations = 1;
    static inline unsigned int MaxIterations = 5;
    unsigned int iterations = 2;

    glm::vec2 getGravity() const { return gravity; };
    void SetPhysicsSettings(PhysicsSettings settings);

    void InitRigidBodies(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive);
    void PhysicsStep(double deltaTime, ActiveTracker<RigidBody*> &physObjects, unsigned int numActive);

private:

    static PhysicsSystem* instance;
    PhysicsSystem(PhysicsSettings settings);
    ~PhysicsSystem() = default;

    void BroadPhase(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive);
    void NarrowPhase(ActiveTracker<RigidBody*> &physObjects);

    glm::vec2 resContacts[2] = {};
    glm::vec2 impulseList[2] = {};
    glm::vec2 raList[2] = {};
    glm::vec2 rbList[2] = {};
    glm::vec2 frictionImpulseList[2] = {};
    float jList[2] = {};

    //TODO: Make a framebuffer for this
    std::vector<std::pair<int, int>> contactList = std::vector<std::pair<int, int>>(20);

    static void SeparateBodies(RigidBody* bodyA, RigidBody* bodyB, const glm::vec2& mtv);
    static void ResolveCollisionBasic(ColManifold& contact);
    void ResolveCollisionComplex(ColManifold& contact);
    glm::vec2 gravity = glm::vec2(0.0f, -9.81f);
};
