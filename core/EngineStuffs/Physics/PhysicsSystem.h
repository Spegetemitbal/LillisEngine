#pragma once

#include <glm/vec2.hpp>

#include "RigidBody.h"
#include "ColManifold.h"
#include "PhysicsEventHandler.h"
#include "EngineStuffs/SpatialHasher.h"
#include "EngineStuffs/Tilemaps/TileCollider.h"
#include "CollisionMatrix.h"

class TileMap;

struct PhysicsSettings
{
    unsigned int physicsIterations = 2;
    glm::vec2 gravity = glm::vec2(0.0f, -9.81f);
};

struct CollisionPairing
{
    CollisionPairing(bool hasTile, std::pair<int, int> colliderIndices, std::pair<int,int> tileCollider = {-1,-1})
    {
        this->hasTile = hasTile;
        this->colliderIndices = colliderIndices;
        this->tileCollider = tileCollider;
    }
    CollisionPairing() = default;
    ~CollisionPairing() = default;
    bool hasTile = false;
    std::pair<int, int> colliderIndices = {-1,-1};
    std::pair<int, int> tileCollider = {-1, -1};
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
    void ChildTriggerUpdate(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive);
    void PhysicsStep(double deltaTime, ActiveTracker<RigidBody*> &physObjects, unsigned int numActive, std::vector<TileMap>& tMaps);

    void DoRenderPhysics(bool rndr)
    {
        renderPhysics = rndr;
    }

    //Get if anything hits.
    bool RayCast(glm::vec2 origin, glm::vec2 direction, float distance = 10, int layerIgnore = -1);
    //Get nearest object hit
    ColManifold RayCastNear(glm::vec2 origin, glm::vec2 direction, float distance = 10, int layerIgnore = -1);
    //Get all objects hit + locations.
    std::vector<ColManifold> RayCastAll(glm::vec2 origin, glm::vec2 direction, float distance = 10, int layerIgnore = -1, unsigned int numCollisions = 0);

    CollisionMatrix collisionMatrix = CollisionMatrix();

    bool GetPhysicsStepComplete() {return physicsStepComplete;}
    void markPhysicsStepIncomplete() {physicsStepComplete = false;}

    static void SeparateBodies(LilObj<RigidBody> bodyA, LilObj<RigidBody> bodyB, const glm::vec2& mtv);

private:
    static PhysicsSystem* instance;
    PhysicsEventHandler* eventHandler = nullptr;
    SpatialHasher spatialHasher = SpatialHasher();
    PhysicsSystem(PhysicsSettings settings);
    ~PhysicsSystem()
    {
        delete eventHandler;
    };

    bool renderPhysics = false;
    bool physicsStepComplete = false;

    void BroadPhase(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive, std::vector<TileMap>& tMaps);
    void NarrowPhase(ActiveTracker<RigidBody*> &physObjects, std::vector<TileMap>& tMaps);

    void HashGrid(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive, std::vector<TileMap>& tMaps);
    void AABBChecks(const CollisionPairing& tempContact, ActiveTracker<RigidBody*> &physObjects, const std::vector<TileCollider>* tMap = nullptr);

    glm::vec2 resContacts[2] = {};
    glm::vec2 impulseList[2] = {};
    glm::vec2 raList[2] = {};
    glm::vec2 rbList[2] = {};
    glm::vec2 frictionImpulseList[2] = {};
    float jList[2] = {};

    //TODO: Make a framebuffer for this
    std::vector<CollisionPairing> contactList = std::vector<CollisionPairing>(20);

    static void SeparateBodies(RigidBody* bodyA, RigidBody* bodyB, const glm::vec2& mtv);
    void ResolveCollisionComplex(ColManifold& contact);
    void ResolveRBCollision(ColManifold& contact);
    void ResolveTileCollision(ColManifold& contact);

    glm::vec2 gravity = glm::vec2(0.0f, -9.81f);
};
