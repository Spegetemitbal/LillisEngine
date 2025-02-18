//
// Created by Somed on 2/17/2025.
//

#include "PhysicsSystem.h"

#include "CollisionChecker.h"

void PhysicsSystem::PhysicsStep(float deltaTime, ActiveTracker<RigidBody*> &physObjects)
{
    //Movement Step
    for (int i = 0; i < physObjects.size(); i++)
    {
        physObjects[i]->Integrate(deltaTime);
        physObjects[i]->UpdateVertices();
    }

    // Collision step
    for (int i = 0; i < physObjects.size() - 1; i++)
    {
        RigidBody* bodyA = physObjects[i];
        if (!bodyA->GetActive())
        {
            continue;
        }

        for (int j = i + 1; j < physObjects.size(); j++)
        {
            RigidBody* bodyB = physObjects[j];
            if (!bodyB->GetActive())
            {
                continue;
            }

            glm::vec2 normal;
            float depth;

            if (CollideCheck(*bodyA, *bodyB, normal, depth))
            {
                //Placeholder
                bodyA->transform->Translate(normal * (depth / 2.0f));
                bodyB->transform->Translate(-normal * (depth / 2.0f));
            }
        }
    }
}

bool PhysicsSystem::CollideCheck(RigidBody bodyA, RigidBody bodyB, glm::vec2 &normal, float &depth)
{
    normal = glm::vec2(0);
    depth = 0;

    RigidBodyShape typeA = bodyA.bodyShape;
    RigidBodyShape typeB = bodyB.bodyShape;

    if (typeA == RigidBodyShape::RB_BOX)
    {
        if (typeA == typeB)
        {
            return CollisionChecker::IntersectPolygons
            (bodyA.GetTransformedVertices(), bodyB.GetTransformedVertices(), normal, depth);
        } else if (typeB == RigidBodyShape::RB_CIRCLE)
        {
            bool result = CollisionChecker::IntersectCirclePolygon
            (bodyB.transform->GlobalPosition(), bodyB.GetRadius(), bodyA.GetTransformedVertices(), normal, depth);
            normal = -normal;
            return result;
        }
    } else if (typeA == RigidBodyShape::RB_CIRCLE)
    {
        if (typeA == typeB)
        {
            CollisionChecker::IntersectCircles
            (bodyA.transform->GlobalPosition(), bodyB.transform->GlobalPosition(),bodyA.GetRadius(), bodyB.GetRadius(), depth, normal);
        } else if (typeB == RigidBodyShape::RB_BOX)
        {
            bool result = CollisionChecker::IntersectCirclePolygon
            (bodyA.transform->GlobalPosition(), bodyA.GetRadius(), bodyB.GetTransformedVertices(), normal, depth);

            normal = -normal;
            return result;
        }
    }

    return false;
}

