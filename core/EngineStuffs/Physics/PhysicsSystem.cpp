//
// Created by Somed on 2/17/2025.
//

#include "PhysicsSystem.h"

#include "CollisionChecker.h"
#include "EngineStuffs/WorldManager.h"
#include "EngineStuffs/Graphics/ProcGen.h"
#include "EngineStuffs/Tilemaps/TileMap.h"

PhysicsSystem* PhysicsSystem::instance = nullptr;

PhysicsSystem *PhysicsSystem::createInstance(PhysicsSettings settings)
{
    if (instance == nullptr)
    {
        instance = new PhysicsSystem(settings);
    }
    return instance;
}


PhysicsSystem *PhysicsSystem::getInstance()
{
    if (instance == nullptr)
    {
        return createInstance();
    }
    return instance;
}


void PhysicsSystem::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

PhysicsSystem::PhysicsSystem(PhysicsSettings settings)
{
    SetPhysicsSettings(settings);
    eventHandler = DBG_NEW PhysicsEventHandler();
}

void PhysicsSystem::SetPhysicsSettings(PhysicsSettings settings)
{
    iterations = settings.physicsIterations;
    gravity = settings.gravity;
}


void PhysicsSystem::InitRigidBodies(ActiveTracker<RigidBody *> &physObjects, unsigned int numActive)
{
    eventHandler->ClearMatrix();
    for (int i = 0; i < numActive; i++)
    {
        physObjects[i]->UpdateVertices();
        physObjects[i]->GetAABB();
    }
}

void PhysicsSystem::ChildTriggerUpdate(ActiveTracker<RigidBody *> &physObjects, unsigned int numActive)
{
    for (int i = 0; i < numActive; i++)
    {
        if (physObjects[i]->transform->getIsChild())
        {
            physObjects[i]->UpdateVertices();
            physObjects[i]->GetAABB();
        }
        if (renderPhysics)
        {
            if (physObjects[i]->bodyShape == RigidBodyShape::RB_BOX)
            {
                BoxData b = physObjects[i]->GetBoxData();
                ProcGen* p = ProcGen::getInstance();
                for (int j = 0; j < 4; j++)
                {
                    p->DrawDebugPoint(b.transformedVertices[j], {255,0,0});
                }
            }
        }
    }
}


void PhysicsSystem::PhysicsStep(double deltaTime, ActiveTracker<RigidBody*> &physObjects, unsigned int numActive, std::vector<TileMap>& tMaps)
{
    if (numActive == 0)
    {
        return;
    }

    //Draw Tilemap procgen stuff.
    if (renderPhysics)
    {
        ProcGen* p = ProcGen::getInstance();
        for (int i = 0; i < tMaps.size(); i++)
        {
            const std::vector<TileCollider> toRender = tMaps[i].getTileColliderVerts();
            for (int j = 0; j < toRender.size(); j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    p->DrawDebugPoint(toRender[j].vertices[k], {255,0,0});
                }
            }
        }
    }

    iterations = std::clamp(iterations, MinIterations, MaxIterations);
    //iterations = 1;
    deltaTime /= (float)iterations;

    for (unsigned int it = 0; it < iterations; it++)
    {
        contactList.clear();

        //Movement Step
        for (int i = 0; i < numActive; i++)
        {
            if (physObjects[i]->GetActive() && physObjects[i]->bodyType != RigidBodyType::RB_STATIC)
            {
                physObjects[i]->UpdateVertices();
                if (!physObjects[i]->isSleeping && !physObjects[i]->isTrigger)
                {
                    physObjects[i]->Integrate((float)deltaTime, gravity);
                }
            }
        }
        // Collision step

        BroadPhase(physObjects, numActive, tMaps);
        NarrowPhase(physObjects, tMaps);
    }
    eventHandler->TickFireEvent(physObjects.getMPool());
    physicsStepComplete = true;
}

//TODO remove.
void PhysicsSystem::BroadPhase(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive, std::vector<TileMap>& tMaps)
{
    HashGrid(physObjects, numActive, tMaps);
}

void PhysicsSystem::HashGrid(ActiveTracker<RigidBody *> &physObjects, unsigned int numActive, std::vector<TileMap> &tMaps)
{
    spatialHasher.Clear();

    bool reloadStaticGeometry = false;
    for (int i = 0; i < tMaps.size(); i++)
    {
        if (tMaps[i].updateColliderFlag && tMaps[i].active)
        {
            reloadStaticGeometry = true;
            break;
        }
    }

    if (reloadStaticGeometry)
    {
        spatialHasher.ClearTiles();
        for (int i = 0; i < tMaps.size(); i++)
        {
            const std::vector<TileCollider>& t = tMaps[i].getTileColliderVerts();
            for (int j = 0; j < t.size(); j++)
            {
                spatialHasher.InsertTile(t[j].aabb, i, j);
            }
            tMaps[i].updateColliderFlag = false;
        }
    }

    for (int i = 0; i < numActive; i++)
    {
        RigidBody* rb = physObjects[i];
        if (rb->GetActive() && rb->GetisEnabled())
        {
            spatialHasher.InsertObject(rb->GetAABB(),i);
        }
    }

    unordered_set<GridHash>& g = spatialHasher.GetGridSpaces();
    //For each gridspace with items in it.
    for (const auto& item : g)
    {
        //Get all items in said grid
        auto gridBits = spatialHasher.GetObjectsInGrid(item);
        auto tileBits = spatialHasher.GetTilesInGrid(item);
        for (auto i = gridBits.first; i != gridBits.second; ++i)
        {
            int firstObject = i->second;
            //If not last, check against all other objects
            if (i != std::prev(gridBits.second))
            {
                for (auto j = std::next(i); j != gridBits.second; ++j)
                {
                    int secondObject = j->second;
                    CollisionPairing p = {false, {firstObject, secondObject}};
                    if (i->first != j->first)
                    {
                        continue;
                        //TODO improve hash!
                    }
                    AABBChecks(p, physObjects, nullptr);
                }
            }

            //Check against all tiles
            if (tileBits.first != tileBits.second && physObjects[firstObject]->bodyType == RigidBodyType::RB_DYNAMIC)
            {
                for (auto j = tileBits.first; j != tileBits.second; ++j)
                {
                    auto pr = j->second;
                    CollisionPairing p = {true, {firstObject, -1}, pr};
                    AABBChecks(p, physObjects, &tMaps[pr.first].getTileColliderVerts());
                }
            }
        }
    }
}

void PhysicsSystem::AABBChecks(const CollisionPairing &tempContact, ActiveTracker<RigidBody*> &physObjects, const std::vector<TileCollider>* tMap)
{
    RigidBody* bodyA = physObjects[tempContact.colliderIndices.first];
    AABB bodyA_aabb = bodyA->GetAABB();
    int colTagA = bodyA->collisionTag;
    if (tempContact.hasTile)
    {
        if (tMap == nullptr)
        {
            return;
        }
        if (collisionMatrix.GetCollisionExclusion(tMap->at(tempContact.tileCollider.second).collisionTag, colTagA))
        {
            return;
        }
        const TileCollider* col = &tMap->at(tempContact.tileCollider.second);
        if (CollisionChecker::IntersectAABBs(bodyA_aabb, col->aabb))
        {
            contactList.push_back(tempContact);
        }
    } else
    {
        RigidBody* bodyB = physObjects[tempContact.colliderIndices.second];
        AABB bodyB_aabb = bodyB->GetAABB();
        int colTagB = bodyB->collisionTag;
        if (collisionMatrix.GetCollisionExclusion(colTagA, colTagB))
        {
            return;
        }
        if (!bodyB->GetActive())
        {
            return;
        }
        if (CollisionChecker::IntersectAABBs(bodyA_aabb, bodyB_aabb))
        {
            contactList.push_back(tempContact);
        }
    }

}



void PhysicsSystem::NarrowPhase(ActiveTracker<RigidBody*> &physObjects, std::vector<TileMap>& tMaps)
{
    for (int i = 0; i < contactList.size(); i++)
    {
        CollisionPairing possibleContact = contactList[i];
        RigidBody* bodyA = nullptr;
        RigidBody* bodyB = nullptr;
        const TileCollider* tile = nullptr;
        if (possibleContact.hasTile)
        {
            bodyA = physObjects[possibleContact.colliderIndices.first];
            std::pair<int,int> tC = possibleContact.tileCollider;
            //TODO Optimize
            tile = &tMaps[tC.first].getTileColliderVerts()[tC.second];

            bool isTriggerCollision = false;
            if (bodyA->isTrigger)
            {
                isTriggerCollision = true;
            } else if (bodyA->transform->GlobalPosition() == tile->center)
            {
                bodyA->transform->Translate({0, 0.1f});
            }

            glm::vec2 normal;
            float depth;

            if (CollisionChecker::CollideCheck(*bodyA, *tile, normal, depth))
            {
                if (!isTriggerCollision)
                {
                    SeparateBodies(bodyA, nullptr, normal * depth);
                }

                int contactCount;
                glm::vec2 contact1, contact2;
                CollisionChecker::FindContactPoints(bodyA, tile, contact1, contact2, contactCount);
                ColManifold cm = ColManifold(bodyA, nullptr, tile, normal, depth, contactCount, contact1, contact2);

                eventHandler->IsColliding(bodyA, cm);

                if (renderPhysics)
                {
                    ProcGen* p = ProcGen::getInstance();
                    if (contactCount > 0)
                    {
                        p->DrawDebugPoint(contact1, {231, 250, 17});
                        if (contactCount > 1)
                        {
                            p->DrawDebugPoint(contact1, {231, 250, 17});
                        }
                    }
                }

                //ResolveCollisionBasic(cm);
                if (!isTriggerCollision)
                {
                    ResolveCollisionComplex(cm);
                }
            }

        }
        else
        {
            bodyA = physObjects[possibleContact.colliderIndices.first];
            bodyB = physObjects[possibleContact.colliderIndices.second];

            bool isTriggerCollision = false;
            if (bodyA->isTrigger || bodyB->isTrigger)
            {
                isTriggerCollision = true;
            } else if (bodyA->transform->GlobalPosition() == bodyB->transform->GlobalPosition())
            {
                bodyA->transform->Translate({0, 0.1f});
            }

            //In the event of an object touching the other, default to popping upwards?

            glm::vec2 normal;
            float depth;

            if (CollisionChecker::CollideCheck(*bodyA, *bodyB, normal, depth))
            {
                if (!isTriggerCollision)
                {
                    SeparateBodies(bodyA, bodyB, normal * depth);
                }

                int contactCount;
                glm::vec2 contact1, contact2;
                CollisionChecker::FindContactPoints(bodyA, bodyB, contact1, contact2, contactCount);
                ColManifold cm = ColManifold(bodyA, bodyB, nullptr, normal, depth, contactCount, contact1, contact2);

                eventHandler->IsColliding(std::make_pair(bodyA, bodyB), cm);

                if (renderPhysics)
                {
                    ProcGen* p = ProcGen::getInstance();
                    if (contactCount > 0)
                    {
                        p->DrawDebugPoint(contact1, {231, 250, 17});
                        if (contactCount > 1)
                        {
                            p->DrawDebugPoint(contact1, {231, 250, 17});
                        }
                    }
                }

                //ResolveCollisionBasic(cm);
                if (!isTriggerCollision)
                {
                    ResolveCollisionComplex(cm);
                }
            }
        }
    }
}

void PhysicsSystem::SeparateBodies(RigidBody *bodyA, RigidBody *bodyB, const glm::vec2 &mtv)
{
    RigidBodyType typeA = bodyA->bodyType;
    if (bodyB == nullptr)
    {
        //Tile Collision
        if (typeA == RigidBodyType::RB_DYNAMIC)
        {
            bodyA->transform->Translate(-mtv * bodyA->GetLinearConstraint());
        }
        return;
    }
    RigidBodyType typeB = bodyB->bodyType;

    if (typeA != RigidBodyType::RB_DYNAMIC && typeB != RigidBodyType::RB_DYNAMIC)
    {
        return;
    }

    if (typeA != RigidBodyType::RB_DYNAMIC)
    {
        bodyB->transform->Translate(mtv * bodyB->GetLinearConstraint());
    } else if (typeB != RigidBodyType::RB_DYNAMIC)
    {
        bodyA->transform->Translate(-mtv * bodyA->GetLinearConstraint());
    } else
    {
        bodyA->transform->Translate((-mtv * bodyA->GetLinearConstraint()) / 2.0f);
        bodyB->transform->Translate((mtv * bodyB->GetLinearConstraint()) / 2.0f);
    }
}

void PhysicsSystem::ResolveCollisionComplex(ColManifold &contact)
{
    if (contact.BodyB == nullptr)
    {
        ResolveTileCollision(contact);
    } else
    {
        ResolveRBCollision(contact);
    }
}

void PhysicsSystem::ResolveRBCollision(ColManifold &contact)
{
    RigidBody* bodyA = contact.BodyA;
    RigidBody* bodyB = contact.BodyB;

    glm::vec2 normal = contact.Normal;
    glm::vec2 contact1 = contact.Contact1;
    glm::vec2 contact2 = contact.Contact2;
    int contactCount = contact.ContactCount;

    //Using min for now, add options to change this.
    float rest = std::min(contact.BodyA->material.restitution, contact.BodyB->material.restitution);

    //TODO: Add options for min or max.
    float sf = (bodyA->material.staticFriction + bodyB->material.staticFriction) * 0.5f;
    float df = (bodyA->material.dynamicFriction + bodyB->material.dynamicFriction) * 0.5f;

    resContacts[0] = contact1;
    resContacts[1] = contact2;

    for (int i = 0; i < contactCount; i++)
    {
        impulseList[i] = {};
        raList[i] = {};
        rbList[i] = {};
        frictionImpulseList[i] = {};
        jList[i] = 0.0f;
    }

    //Handle normal collision impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 ra = resContacts[i] - bodyA->transform->GlobalPosition();
        glm::vec2 rb = resContacts[i] - bodyB->transform->GlobalPosition();

        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
        glm::vec2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

        glm::vec2 relativeVelocity =
            (bodyB->linearVelocity + angularLinearVelocityB) -
            (bodyA->linearVelocity + angularLinearVelocityA);

        float contactVelocityMag = glm::dot(relativeVelocity, normal);

        if (contactVelocityMag > 0.0f)
        {
            continue;
        }

        float raPerpDotN = glm::dot(raPerp, normal);
        float rbPerpDotN = glm::dot(rbPerp, normal);

        float denom = bodyA->invMass + bodyB->invMass +
            (raPerpDotN * raPerpDotN) * bodyA->invInertia +
            (rbPerpDotN * rbPerpDotN) * bodyB->invInertia;

        float j = -(1.0f + rest) * contactVelocityMag;
        j /= denom;
        j /= (float)contactCount;

        jList[i] = j;

        glm::vec2 impulse = j * normal;
        impulseList[i] = impulse;
    }

    //Use impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 impulse = impulseList[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        bodyA->AddImpulse(-impulse * bodyA->invMass, -(ra.x * impulse.y - impulse.x * ra.y) * bodyA->invInertia);
        bodyB->AddImpulse(impulse * bodyB->invMass, (rb.x * impulse.y - impulse.x * rb.y) * bodyB->invInertia);
    }

    //Calculate friction impulse
    for (int i = 0; i < contactCount; i++)
    {

        glm::vec2 ra = resContacts[i] - bodyA->transform->GlobalPosition();
        glm::vec2 rb = resContacts[i] - bodyB->transform->GlobalPosition();

        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
        glm::vec2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

        glm::vec2 relativeVelocity =
            (bodyB->linearVelocity + angularLinearVelocityB) -
            (bodyA->linearVelocity + angularLinearVelocityA);

        glm::vec2 tangent = relativeVelocity - glm::dot(relativeVelocity, normal) * normal;

        if (CollisionChecker::GetNearlyEqual(tangent, {}))
        {
            continue;
        }
        tangent = glm::normalize(tangent);

        float raPerpDotT = glm::dot(raPerp, tangent);
        float rbPerpDotT = glm::dot(rbPerp, tangent);

        float denom = bodyA->invMass + bodyB->invMass +
            (raPerpDotT * raPerpDotT) * bodyA->invInertia +
                (rbPerpDotT * rbPerpDotT) * bodyB->invInertia;

        float jt = -glm::dot(relativeVelocity, tangent);
        jt /= denom;
        jt /= (float)contactCount;

        //No tiny stuff.
        //if (jt < RigidBody::EPSILON)
        //{
        //    continue;
        //}

        glm::vec2 impulseFric;
        float j = jList[i];

        if (std::abs(jt) <= j * sf)
        {
            impulseFric = jt * tangent;
        } else
        {
            impulseFric = -j * tangent * df;
        }

        frictionImpulseList[i] = impulseFric;
    }

    //Activate friction impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 frictionImpulse = frictionImpulseList[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        //TODO: Make a cross product function-
        // cz = ax * by − ay * bx
        bodyA->AddImpulse(-frictionImpulse * bodyA->invMass
            , -((ra.x * frictionImpulse.y) - (frictionImpulse.x * ra.y)) * bodyA->invInertia);
        bodyB->AddImpulse(frictionImpulse * bodyB->invMass
            , ((rb.x * frictionImpulse.y) - (frictionImpulse.x * rb.y)) * bodyB->invInertia);
    }
}

void PhysicsSystem::ResolveTileCollision(ColManifold &contact)
{
    RigidBody* bodyA = contact.BodyA;
    const TileCollider* tile = contact.Tile;

    glm::vec2 normal = contact.Normal;
    glm::vec2 contact1 = contact.Contact1;
    glm::vec2 contact2 = contact.Contact2;
    int contactCount = contact.ContactCount;

    //Using min for now, add options to change this.
    float rest = std::min(contact.BodyA->material.restitution, tile->material->restitution);

    //TODO: Add options for min or max.
    float sf = (bodyA->material.staticFriction + tile->material->staticFriction) * 0.5f;
    float df = (bodyA->material.dynamicFriction + tile->material->dynamicFriction) * 0.5f;

    resContacts[0] = contact1;
    resContacts[1] = contact2;

    for (int i = 0; i < contactCount; i++)
    {
        impulseList[i] = {};
        raList[i] = {};
        rbList[i] = {};
        frictionImpulseList[i] = {};
        jList[i] = 0.0f;
    }

    //Handle normal collision impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 ra = resContacts[i] - bodyA->transform->GlobalPosition();
        glm::vec2 rb = resContacts[i] - tile->center;

        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
        //glm::vec2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

        glm::vec2 relativeVelocity =
            -(bodyA->linearVelocity + angularLinearVelocityA);

        float contactVelocityMag = glm::dot(relativeVelocity, normal);

        if (contactVelocityMag > 0.0f)
        {
            continue;
        }

        float raPerpDotN = glm::dot(raPerp, normal);
        float rbPerpDotN = glm::dot(rbPerp, normal);

        float denom = bodyA->invMass +
            (raPerpDotN * raPerpDotN) * bodyA->invInertia;

        float j = -(1.0f + rest) * contactVelocityMag;
        j /= denom;
        j /= (float)contactCount;

        jList[i] = j;

        glm::vec2 impulse = j * normal;
        impulseList[i] = impulse;
    }

    //Use impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 impulse = impulseList[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        bodyA->AddImpulse(-impulse * bodyA->invMass, -(ra.x * impulse.y - impulse.x * ra.y) * bodyA->invInertia);
    }

    //Calculate friction impulse
    for (int i = 0; i < contactCount; i++)
    {

        glm::vec2 ra = resContacts[i] - bodyA->transform->GlobalPosition();
        glm::vec2 rb = resContacts[i] - tile->center;

        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;

        glm::vec2 relativeVelocity =
            -(bodyA->linearVelocity + angularLinearVelocityA);

        glm::vec2 tangent = relativeVelocity - glm::dot(relativeVelocity, normal) * normal;

        if (CollisionChecker::GetNearlyEqual(tangent, {}))
        {
            continue;
        }
        tangent = glm::normalize(tangent);

        float raPerpDotT = glm::dot(raPerp, tangent);
        float rbPerpDotT = glm::dot(rbPerp, tangent);

        float denom = bodyA->invMass +
            (raPerpDotT * raPerpDotT) * bodyA->invInertia;

        float jt = -glm::dot(relativeVelocity, tangent);
        jt /= denom;
        jt /= (float)contactCount;

        //No tiny stuff.
        //if (jt < RigidBody::EPSILON)
        //{
        //    continue;
        //}

        glm::vec2 impulseFric;
        float j = jList[i];

        if (std::abs(jt) <= j * sf)
        {
            impulseFric = jt * tangent;
        } else
        {
            impulseFric = -j * tangent * df;
        }

        frictionImpulseList[i] = impulseFric;
    }

    //Activate friction impulse
    for (int i = 0; i < contactCount; i++)
    {
        glm::vec2 frictionImpulse = frictionImpulseList[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        //TODO: Make a cross product function-
        // cz = ax * by − ay * bx
        bodyA->AddImpulse(-frictionImpulse * bodyA->invMass
            , -((ra.x * frictionImpulse.y) - (frictionImpulse.x * ra.y)) * bodyA->invInertia);
    }
}

bool PhysicsSystem::RayCast(glm::vec2 origin, glm::vec2 direction, float distance, int layerIgnore)
{
    if (!physicsStepComplete)
    {
        std::cout << "Cannot perform Raycasts before physics are generated for the frame, try using LateUpdate" << std::endl;
        return false;
    }

    GameWorld* world = WorldManager::getInstance()->GetCurrentWorld();
    if (world == nullptr)
    {
        std::cout << "No world exists to perform raycast" << std::endl;
        return false;
    }

    ActiveTracker<RigidBody*> rbs = world->getRBsRaw();
    std::vector<TileMap>& mps = world->getTileMaps();

    //Generate ray info.
    if (glm::length(direction) != 1)
    {
        if (glm::length(direction) <= 0.0f)
        {
            std::cout << "Non-zero direction vector required" << std::endl;
            return false;
        }
        direction = glm::normalize(direction);
    }

    glm::vec2 endPoint = origin + (direction * distance);

    glm::vec2 lineData[2] = {{std::min(origin.x, endPoint.x), std::min(origin.y, endPoint.y)},
        {std::max(origin.x, endPoint.x), std::max(origin.y, endPoint.y)}};

    AABB lineAABB = AABB(lineData[0], lineData[1]);
    std::vector<GridHash> placesToCheck = spatialHasher.GetGridSpacesOfObject(lineAABB);

    for (int i = 0; i < placesToCheck.size(); i++)
    {
        auto gridBits = spatialHasher.GetObjectsInGrid(placesToCheck[i]);
        auto tileBits = spatialHasher.GetTilesInGrid(placesToCheck[i]);

        //Now check against all objects.
        for (auto it = gridBits.first; it != gridBits.second; ++i)
        {
            if (!rbs[it->second]->GetActive() || rbs[it->second]->collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(*rbs[it->second], lineData))
            {
                return true;
            }
        }

        //Check against all tiles
        for (auto j = tileBits.first; j != tileBits.second; ++j)
        {
            auto pr = j->second;
            if (!mps[pr.first].active || mps[pr.first].collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(mps[pr.first].getTileColliderVerts().at(pr.second), lineData))
            {
                return true;
            }
        }

    }

    return false;
}

ColManifold PhysicsSystem::RayCastNear(glm::vec2 origin, glm::vec2 direction, float distance, int layerIgnore)
{
    ColManifold cm = ColManifold();

    if (!physicsStepComplete)
    {
        std::cout << "Cannot perform Raycasts before physics are generated for the frame, try using LateUpdate" << std::endl;
        return cm;
    }

    GameWorld* world = WorldManager::getInstance()->GetCurrentWorld();
    if (world == nullptr)
    {
        std::cout << "No world exists to perform raycast" << std::endl;
        return cm;
    }

    ActiveTracker<RigidBody*> rbs = world->getRBsRaw();
    std::vector<TileMap>& mps = world->getTileMaps();

    //Generate ray info.
    if (glm::length(direction) != 1)
    {
        if (glm::length(direction) <= 0.0f)
        {
            std::cout << "Non-zero direction vector required" << std::endl;
            return cm;
        }
        direction = glm::normalize(direction);
    }

    glm::vec2 endPoint = origin + (direction * distance);

    glm::vec2 lineData[2] = {{std::min(origin.x, endPoint.x), std::min(origin.y, endPoint.y)},
        {std::max(origin.x, endPoint.x), std::max(origin.y, endPoint.y)}};

    AABB lineAABB = AABB(lineData[0], lineData[1]);
    std::vector<GridHash> placesToCheck = spatialHasher.GetGridSpacesOfObject(lineAABB);

    //Find direction of vector, sort gridspaces by said direction.
    int xDir = direction.x > 0 ? 1 : -1;
    int yDir = direction.y > 0 ? 1 : -1;
    std::sort(placesToCheck.begin(), placesToCheck.end(),
        [xDir, yDir](const GridHash& a, const GridHash& b)
        {
            int aX = a.x * xDir, aY = a.y * yDir, bX = b.x * xDir, bY = b.y * yDir;
            return aX < bX && aY <= bY || aY < bY && aX <= bX;
        });


    for (int i = 0; i < placesToCheck.size(); i++)
    {
        auto gridBits = spatialHasher.GetObjectsInGrid(placesToCheck[i]);
        auto tileBits = spatialHasher.GetTilesInGrid(placesToCheck[i]);

        //Now check against all objects.
        for (auto it = gridBits.first; it != gridBits.second; ++i)
        {
            if (!rbs[it->second]->GetActive() || rbs[it->second]->collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(*rbs[it->second], lineData))
            {
                CollisionChecker::GetRayContacts(rbs[it->second], lineData, cm.Contact1, cm.Contact2, cm.ContactCount);
                cm.BodyA = rbs[it->second];
                return cm;
            }
        }

        //Check against all tiles
        for (auto j = tileBits.first; j != tileBits.second; ++j)
        {
            auto pr = j->second;
            if (!mps[pr.first].active || mps[pr.first].collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(mps[pr.first].getTileColliderVerts().at(pr.second), lineData))
            {
                CollisionChecker::GetRayContacts(&mps[pr.first].getTileColliderVerts().at(pr.second), lineData, cm.Contact1, cm.Contact2, cm.ContactCount);
                cm.Tile = &mps[pr.first].getTileColliderVerts().at(pr.second);
                return cm;
            }
        }

    }

    return cm;
}

std::vector<ColManifold> PhysicsSystem::RayCastAll(glm::vec2 origin, glm::vec2 direction, float distance, int layerIgnore, unsigned int numCollisions)
{
    std::vector<ColManifold> cm;

    if (!physicsStepComplete)
    {
        std::cout << "Cannot perform Raycasts before physics are generated for the frame, try using LateUpdate" << std::endl;
        return cm;
    }

    GameWorld* world = WorldManager::getInstance()->GetCurrentWorld();
    if (world == nullptr)
    {
        std::cout << "No world exists to perform raycast" << std::endl;
        return cm;
    }

    ActiveTracker<RigidBody*> rbs = world->getRBsRaw();
    std::vector<TileMap>& mps = world->getTileMaps();

    //Generate ray info.
    if (glm::length(direction) != 1)
    {
        direction = glm::normalize(direction);
    }

    glm::vec2 endPoint = origin + (direction * distance);

    glm::vec2 lineData[2] = {{std::min(origin.x, endPoint.x), std::min(origin.y, endPoint.y)},
        {std::max(origin.x, endPoint.x), std::max(origin.y, endPoint.y)}};

    AABB lineAABB = AABB(lineData[0], lineData[1]);
    std::vector<GridHash> placesToCheck = spatialHasher.GetGridSpacesOfObject(lineAABB);

    for (int i = 0; i < placesToCheck.size(); i++)
    {
        auto gridBits = spatialHasher.GetObjectsInGrid(placesToCheck[i]);
        auto tileBits = spatialHasher.GetTilesInGrid(placesToCheck[i]);

        //Now check against all objects.
        for (auto it = gridBits.first; it != gridBits.second; ++i)
        {
            if (!rbs[it->second]->GetActive() || rbs[it->second]->collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(*rbs[it->second], lineData))
            {
                cm.emplace_back();
                cm.back().BodyA = rbs[it->second];
                CollisionChecker::GetRayContacts(rbs[it->second], lineData, cm.back().Contact1, cm.back().Contact2, cm.back().ContactCount);
            }
        }

        //Check against all tiles
        for (auto j = tileBits.first; j != tileBits.second; ++j)
        {
            auto pr = j->second;
            if (!mps[pr.first].active || mps[pr.first].collisionTag == layerIgnore)
            {
                continue;
            }
            if (CollisionChecker::RayCastCheck(mps[pr.first].getTileColliderVerts().at(pr.second), lineData))
            {
                cm.emplace_back();
                cm.back().Tile = &mps[pr.first].getTileColliderVerts().at(pr.second);
                CollisionChecker::GetRayContacts(&mps[pr.first].getTileColliderVerts().at(pr.second), lineData, cm.back().Contact1, cm.back().Contact2, cm.back().ContactCount);
            }
        }

    }

    return cm;
}




