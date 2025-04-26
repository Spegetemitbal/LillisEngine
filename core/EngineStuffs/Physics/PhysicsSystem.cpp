//
// Created by Somed on 2/17/2025.
//

#include "PhysicsSystem.h"

#include <glm/detail/func_geometric.inl>

#include "CollisionChecker.h"

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
}

void PhysicsSystem::SetPhysicsSettings(PhysicsSettings settings)
{
    iterations = settings.physicsIterations;
    gravity = settings.gravity;
}


void PhysicsSystem::InitRigidBodies(ActiveTracker<RigidBody *> &physObjects, unsigned int numActive)
{
    for (unsigned int i = 0; i < numActive; i++)
    {
        physObjects[i]->UpdateVertices();
        physObjects[i]->GetAABB();
    }
}


void PhysicsSystem::PhysicsStep(float deltaTime, ActiveTracker<RigidBody*> &physObjects, unsigned int numActive)
{
    if (numActive == 0)
    {
        return;
    }


    iterations = std::clamp(iterations, MinIterations, MaxIterations);
    contactList.clear();
    deltaTime /= (float)iterations;

    for (unsigned int it = 0; it < iterations; it++)
    {
        contactList.clear();

        //Movement Step
        for (int i = 0; i < numActive; i++)
        {
            if (physObjects[i]->GetActive() && physObjects[i]->bodyType != RigidBodyType::RB_STATIC)
            {
                physObjects[i]->Integrate(deltaTime, gravity);
                physObjects[i]->UpdateVertices();
            }
        }

        // Collision step

        BroadPhase(physObjects, numActive);
        NarrowPhase(physObjects);

    }
}

void PhysicsSystem::BroadPhase(ActiveTracker<RigidBody*> &physObjects, unsigned int numActive)
{
    for (int i = 0; i < numActive - 1; i++)
    {

        RigidBody* bodyA = physObjects[i];
        AABB bodyA_aabb = bodyA->GetAABB();
        if (!bodyA->GetActive())
        {
            continue;
        }

        for (int j = i + 1; j < numActive; j++)
        {
            RigidBody* bodyB = physObjects[j];
            AABB bodyB_aabb = bodyB->GetAABB();
            if (!bodyB->GetActive())
            {
                continue;
            }

            if (!CollisionChecker::IntersectAABBs(bodyA_aabb, bodyB_aabb))
            {
                continue;
            }

            contactList.emplace_back(i,j);
        }
    }
}

void PhysicsSystem::NarrowPhase(ActiveTracker<RigidBody*> &physObjects)
{
    for (int i = 0; i < contactList.size(); i++)
    {
        std::pair<int, int> possibleContact = contactList[i];
        RigidBody* bodyA = physObjects[possibleContact.first];
        RigidBody* bodyB = physObjects[possibleContact.second];

        //In the event of an object touching the other, default to popping upwards?
        if (bodyA->transform->GlobalPosition() == bodyB->transform->GlobalPosition())
        {
            bodyA->transform->Translate({0, 0.1f});
        }

        glm::vec2 normal;
        float depth;

        if (CollisionChecker::CollideCheck(*bodyA, *bodyB, normal, depth))
        {
            SeparateBodies(bodyA, bodyB, normal * depth);

            int contactCount;
            glm::vec2 contact1, contact2;
            CollisionChecker::FindContactPoints(bodyA, bodyB, contact1, contact2, contactCount);
            ColManifold cm = ColManifold(bodyA, bodyB, normal, depth, contactCount, contact1, contact2);
            //ResolveCollisionBasic(cm);
            ResolveCollisionBasic(cm);
        }
    }
}


void PhysicsSystem::SeparateBodies(RigidBody *bodyA, RigidBody *bodyB, const glm::vec2 &mtv)
{
    RigidBodyType typeA = bodyA->bodyType;
    RigidBodyType typeB = bodyB->bodyType;

    if (typeA != RigidBodyType::RB_DYNAMIC && typeB != RigidBodyType::RB_DYNAMIC)
    {
        return;
    }

    if (typeA != RigidBodyType::RB_DYNAMIC)
    {
        bodyB->transform->Translate(mtv);
    } else if (typeB != RigidBodyType::RB_DYNAMIC)
    {
        bodyA->transform->Translate(-mtv);
    } else
    {
        bodyA->transform->Translate(-mtv / 2.0f);
        bodyB->transform->Translate(mtv / 2.0f);
    }
}


void PhysicsSystem::ResolveCollisionBasic(ColManifold& contact)
{
    //Double statics touching would be a guarantee of UH-OH.
    if (contact.BodyA->bodyType == RigidBodyType::RB_STATIC && contact.BodyB->bodyType == RigidBodyType::RB_STATIC)
    {
        return;
    }

    glm::vec2 relativeVelocity = contact.BodyB->linearVelocity - contact.BodyA->linearVelocity;

    if (glm::dot(relativeVelocity, contact.Normal) > 0)
    {
        return;
    }

    //Using min for now, add options to change this.
    float rest = std::min(contact.BodyA->material.restitution, contact.BodyB->material.restitution);

    float j = -(1.0f + rest) * glm::dot(relativeVelocity, contact.Normal);

    j /= contact.BodyA->invMass + contact.BodyB->invMass;

    glm::vec2 impulse = j * contact.Normal;

    contact.BodyA->linearVelocity -= impulse * contact.BodyA->invMass;
    contact.BodyB->linearVelocity += impulse * contact.BodyB->invMass;
}

void PhysicsSystem::ResolveCollisionComplex(ColManifold &contact)
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

    //Handle normal collision impulse
    for (int i = 0; i < contactCount; i++)
    {
        impulseList[i] = {};
        raList[i] = {};
        rbList[i] = {};
        frictionImpulseList[i] = {};
        jList[i] = 0.0f;

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

        bodyA->linearVelocity += -impulse * bodyA->invMass;
        bodyA->angularVelocity += -(ra.x * impulse.y - impulse.x * ra.y) * bodyA->invInertia;
        bodyB->linearVelocity += impulse * bodyB->invMass;
        bodyB->angularVelocity += (rb.x * impulse.y - impulse.x * rb.y) * bodyB->invInertia;
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

        glm::vec2 tangent = relativeVelocity - (glm::dot(relativeVelocity, normal) * normal);

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
        //bodyA->linearVelocity += -frictionImpulse * bodyA->invMass;
        //bodyA->angularVelocity += -((ra.x * frictionImpulse.y) - (frictionImpulse.x * ra.y)) * bodyA->invInertia;
        //bodyB->linearVelocity += frictionImpulse * bodyB->invMass;
        //bodyB->angularVelocity += ((rb.x * frictionImpulse.y) - (frictionImpulse.x * rb.y)) * bodyB->invInertia;
    }
}


