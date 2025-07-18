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
    FORCE,
    HINGE,
    CABLE,
    BUNGEE
};

class PhysJoint : public Component
{
public:
    void SetBodyA(LilObj<RigidBody> body);
    void SetBodyB(LilObj<RigidBody> body);

private:
    JOINT_TYPES jType = FIXED;

    float breakForce = 0.0f;
    bool doBreaking = false;

    LilObj<RigidBody> bodyA;
    LilObj<RigidBody> bodyB;
};



#endif //PHYSJOINT_H
