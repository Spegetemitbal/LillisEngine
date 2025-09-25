//
// Created by Somed on 7/17/2025.
//

#include "PhysJoint.h"

#include "EngineStuffs/GameObject.h"

void PhysJoint::ConnectComponents()
{
    if (thisObject.Exists())
    {
        LilObj<RigidBody> rb = thisObject->getRigidBody();
        if (rb.Exists())
        {
            SetBodyA(rb);
        }
    }
}

void PhysJoint::SetBodyA(LilObj<RigidBody> body)
{
    bodyA = body;
    if (bodyB.Exists() && bodyA.Exists() && jType != JOINT_TYPES::HINGE)
    {
        relativePos = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
        relativeOrient = bodyB->transform->GlobalRotation() - bodyA->transform->GlobalRotation();
    }
}

void PhysJoint::SetBodyB(LilObj<RigidBody> body)
{
    bodyB = body;
    if (bodyA.Exists() && bodyB.Exists() && jType != JOINT_TYPES::HINGE)
    {
        relativePos = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
        relativeOrient = bodyB->transform->GlobalRotation() - bodyA->transform->GlobalRotation();
    }
}

void PhysJoint::RunJointCollision(float dt)
{
    if (!bodyB.Exists() || !bodyA.Exists())
    {
        return;
    }
    if (!bodyA->GetActive() || !bodyB->GetActive())
    {
        return;
    }
    if (bodyA->bodyType != RigidBodyType::RB_DYNAMIC && bodyB->bodyType != RigidBodyType::RB_DYNAMIC)
    {
        return;
    }

    glm::vec2 dir, finalForce;
    float dist, restDist, forceMag = 0, depth, Sin, Cos, orientDiff;

    //Yeah I know this isn't the OOP way of doing this, but memory management demands it.
    switch (jType)
    {
        case JOINT_TYPES::FIXED:
            //Snap to place
                //TODO add wiggle to rotation if need be.
                if (bodyA->bodyType != RigidBodyType::RB_DYNAMIC || bodyB->bodyType != RigidBodyType::RB_DYNAMIC)
                {
                    break;
                }

                orientDiff = (bodyB->transform->GlobalRotation() - bodyA->transform->GlobalRotation()) - relativeOrient;
                dir = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();

                if (std::abs(orientDiff) > MIN_THRESHOLD)
                {
                    orientDiff *= 0.5f;
                    glm::vec2 centerPoint = bodyA->transform->GlobalPosition() + (dir * 0.5f);
                    //body B rotates half in the minus direction, body A rotates half in the positive.
                    bodyB->transform->RotateAroundPoint(-orientDiff,centerPoint);
                    bodyA->transform->RotateAroundPoint(orientDiff,centerPoint);
                    //May need to flip this.
                    relativePos = {relativePos.x * glm::cos(orientDiff) - relativePos.y * -glm::sin(orientDiff),
                    relativePos.x * glm::sin(orientDiff) - relativePos.y * glm::cos(orientDiff)};
                    dir = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
                }
                glm::vec2 correctionDir = dir - relativePos;
                dist = glm::length(correctionDir);
                if (dist <= MIN_THRESHOLD)
                {
                    break;
                }
                forceMag = dist;
                //Also might need to flip this
                PhysicsSystem::SeparateBodies(bodyA, bodyB, correctionDir);
            break;
        case JOINT_TYPES::CABLE:
            //collide if too far apart, fixing them together.
                dir = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
                dist = glm::length(dir);
                depth = dist - joint_distance;
                if (depth <= 0.0f)
                {
                    break;
                }
                //break force is max depth.
                forceMag = depth;
                PhysicsSystem::SeparateBodies(bodyA, bodyB, glm::normalize(dir) * depth);
            break;
        case JOINT_TYPES::ATTRACT:
            //pull together
                dir = glm::normalize(bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition());
                bodyA->AddForce(dir * (joint_strength * 0.5f));
                bodyB->AddForce(-dir * (joint_strength * 0.5f));
            break;
        case JOINT_TYPES::REPEL:
            //push apart
                dir = glm::normalize(bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition());
                bodyA->AddForce(-dir * (joint_strength * 0.5f));
                bodyB->AddForce(dir * (joint_strength * 0.5f));
            break;
        case JOINT_TYPES::HINGE:
            //doorhinge time.
                //Ensure anchor point is rotated correctly.
                Sin = glm::sin(bodyA->transform->GlobalRotation() - relativeOrient);
                Cos = glm::cos(bodyA->transform->GlobalRotation() - relativeOrient);
                glm::vec2 anchor = {
                    Cos * relativePos.x - Sin * relativePos.y + bodyA->transform->GlobalPosition().x,
                    Sin * relativePos.x + Cos * relativePos.y + bodyA->transform->GlobalPosition().y
                    };
                dir = bodyB->transform->GlobalPosition() - anchor;
                dist = glm::length(dir);
                if (dist <= MIN_THRESHOLD)
                {
                    break;
                }
                forceMag = dist;
                //Also might need to flip this
                PhysicsSystem::SeparateBodies(bodyA, bodyB, dir);
            break;
        case JOINT_TYPES::BUNGEE:
            //Boing but one way
                dir = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
                dist = glm::length(dir);
                restDist = dist - joint_distance;
                if (restDist < 0.01f)
                {
                    //Early out
                    break;
                }
                forceMag = restDist * joint_distance * joint_strength;
                dir = glm::normalize(dir);
                finalForce = dir * forceMag;
                bodyA->AddForce(finalForce);
                bodyB->AddForce(-finalForce);
            break;
        case JOINT_TYPES::SPRING:
            //Boioioing
                dir = bodyB->transform->GlobalPosition() - bodyA->transform->GlobalPosition();
                dist = glm::length(dir);
                restDist = dist - joint_distance;
                if (std::abs(restDist) < 0.01f)
                {
                    //Early out.
                    break;
                }
                forceMag = restDist * joint_distance * joint_strength;
                dir = glm::normalize(dir);
                finalForce = dir * forceMag;
                bodyA->AddForce(finalForce);
                bodyB->AddForce(-finalForce);
            break;
    }

    bodyA->UpdateVertices();
    bodyB->UpdateVertices();

    if (doBreaking)
    {
        if (std::abs(forceMag) > breakForce)
        {

        }
    }
}

