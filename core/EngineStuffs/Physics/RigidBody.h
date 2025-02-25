//
// Created by Somed on 2/15/2025.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "EngineStuffs/Component.h"
#include "EngineStuffs/Transform.h"
#include "AABB.h"

enum RigidBodyType
{
    RB_DYNAMIC,
    RB_KINEMATIC,
    RB_STATIC
};

enum RigidBodyShape
{
    RB_CIRCLE,
    RB_BOX
};

class RigidBody : public Component {
public:
    RigidBody() = default;
    ~RigidBody() = default;

    void SetMass(float mass);
    void SetDensity(float density);
    void SetRestitution(float restitution);

    void SetSize(glm::vec2 size);
    glm::vec2 GetSize() { return size; };
    float GetRadius() {return radius;};
    void InitVertices();

    glm::vec2 LinearVelocity() const {return linearVelocity;}
    float AngularVelocity() const {return angularVelocity;}

    void UpdateVertices();

    void Integrate(float deltaTime, glm::vec2 gravity);

    void AddForce(glm::vec2 force)
    {
        accumulatedForce += force;
    }
    void SetForce(glm::vec2 force)
    {
        accumulatedForce = force;
    }

    AABB GetAABB();

    inline static const int triangles[6] = {0,1,2,0,2,3};
    glm::vec2* GetTransformedVertices() {return transformedVertices;}

    RigidBodyType bodyType = RB_DYNAMIC;
    RigidBodyShape bodyShape = RB_BOX;

    float Radius() const {return radius;}

    float gravityScale = 1.0f;

    //Make sure this sucker is assigned
    LilObj<Transform> transform;

private:

    friend class PhysicsSystem;

    glm::vec2 linearVelocity = glm::vec2(0);
    float angularVelocity = 0.0f;
    glm::vec2 accumulatedForce = glm::vec2(0);

    float inertia = 0.0f;
    float invInertia = 0.0f;
    float CalculateRotationalInertia();

    float density = 1.0f;
    float mass = 1.0f;
    float invMass = 1.0f;
    void CalcInvMass();
    //Bounciness, between 0 and 1
    float restitution = 0;
    float area = 1;

    //TODO: Make this offset the center!!
    glm::vec2 centerOfMass = glm::vec2(0);

    glm::vec2 vertices[4];
    glm::vec2 transformedVertices[4];
    AABB aabb;
    //Hook up to gameobject my guy.

    float radius = 1;
    glm::vec2 size = glm::vec2(1.0f);
    float StaticFriction = 0.6f;
    float DynamicFriction = 0.4f;

    static glm::vec2 transformVertex(glm::vec2 v, glm::vec2 tr, float r);
};



#endif //RIGIDBODY_H
