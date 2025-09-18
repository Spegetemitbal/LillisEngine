//
// Created by Somed on 7/17/2025.
//

#ifndef PHYSJOINT_H
#define PHYSJOINT_H
#include "RigidBody.h"
#include "EngineStuffs/Component.h"

enum JOINT_TYPES
{
    FIXED = 0,
    SPRING,
    ATTRACT,
    REPEL,
    HINGE,
    CABLE,
    BUNGEE
};

class PhysJoint : public Component
{
public:
    void ConnectComponents();
    void SetBodyB(LilObj<RigidBody> body);

    void RunJointCollision(float dt);

    void SetStrength(float strength)
    {
        joint_strength = strength;
        if (joint_strength == 0.0f)
        {
            joint_strength = 1.0f;
        }
    }
    float GetStrength() const { return joint_strength;}
    void SetDistance(float distance)
    {
        joint_distance = distance;
        if (joint_distance <= 0.0f)
        {
            joint_distance = 1.0f;
        }
    }
    float GetDistance() const {return joint_distance;}

    void SetBreaking(float force, bool breaking)
    {
        breakForce = force;
        if (breakForce <= 0.0f && breaking)
        {
            breakForce = 1.0f;
        }
        doBreaking = breaking;
    }

    void SetAnchorPoint(glm::vec2 anchor)
    {
        if (!bodyA.Exists())
        {
            return;
        }

        if (jType == JOINT_TYPES::HINGE)
        {
            relativePos = anchor;
            relativeOrient = bodyA->transform->GlobalRotation();
            return;
        }
        std::cout << "Non-Hinge cannot have anchor point" << std::endl;
    }

    void SetJointType(JOINT_TYPES jt)
    {
        jType = jt;
        SetBodyA(bodyA);
    }
    JOINT_TYPES GetJointType() const {return jType;}


private:

    void SetBodyA(LilObj<RigidBody> body);

    JOINT_TYPES jType = FIXED;

    float joint_distance = 5.0f;
    float joint_strength = 1.0f;

    float breakForce = 10.0f;
    bool doBreaking = false;

    LilObj<RigidBody> bodyA;
    LilObj<RigidBody> bodyB;

    glm::vec2 relativePos = {};
    float relativeOrient = 0.0f;

    static inline float MIN_THRESHOLD = 0.0001f;
};



#endif //PHYSJOINT_H
