//
// Created by Somed on 2/15/2025.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "EngineStuffs/Component.h"
#include "EngineStuffs/Transform.h"
#include "PhysicsMaterial.h"
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

enum RigidBodyConstraint
{
    RB_XVEL,
    RB_YVEL,
    RB_ALLVEL,
    RB_ROT
};

struct CircleData
{
    float radius = 1;
};

struct BoxData
{
    glm::vec2 vertices[4] = {};
    glm::vec2 transformedVertices[4] = {};
    glm::vec2 boxSize = glm::vec2(1.0f);
};

class RigidBody : public Component {
public:
    RigidBody();
    ~RigidBody() = default;

    void SetMass(float mass);
    void SetDensity(float density);
    void SetRestitution(float restitution);

    void SetSize(glm::vec2 newSize);
    glm::vec2 GetSize() { return boxData.boxSize; };
    void SetRadius(float rad) {circleData.radius = rad;}
    float GetRadius() {return circleData.radius;};
    int GetNumVertices() {return numVertices;}
    void InitVertices();

    void SetLinearVelocity(glm::vec2 velocity) {linearVelocity = velocity;}
    glm::vec2 LinearVelocity() const {return linearVelocity;}
    void SetAngularVelocity(float velocity) {angularVelocity = velocity;}
    float AngularVelocity() const {return angularVelocity;}

    float DynamicFriction() const {return material.dynamicFriction;}
    float StaticFriction() const {return material.staticFriction;}
    void SetDynamicFriction(float fric);
    void SetStaticFriction(float fric);

    void UpdateVertices();

    void Integrate(float deltaTime, glm::vec2 gravity);

    void AddForce(glm::vec2 force);
    void SetForce(glm::vec2 force);
    void AddImpulse(glm::vec2 impulse, float torque = 0);

    BoxData GetBoxData() {return boxData;}
    CircleData GetCircleData() {return circleData;}

    void SetLinearDamping(float damping)
    {
        damping = std::clamp(damping, 0.01f, 1.0f);
        linearDamping = 1.0f - damping;
    }
    void SetAngularDamping(float damping)
    {
        damping = std::clamp(damping, 0.01f, 1.0f);
        angularDamping = 1.0f - damping;
    }
    float GetLinearDamping() const
    {
        return 1.0f - linearDamping;
    }
    float GetAngularDamping() const
    {
        return 1.0f - angularDamping;
    }

    int GetColTag() const
    {
        return collisionTag;
    }
    void SetColTag(int tag)
    {
        collisionTag = tag;
    }

    void SetConstraint(RigidBodyConstraint constraint, bool val);
    float GetConstraintMultiplier(RigidBodyConstraint constraint) const;
    glm::vec2 GetLinearConstraint() const {return linearConstraints;}

    AABB GetAABB();

    inline static const int triangles[6] = {0,1,2,0,2,3};
    glm::vec2* GetTransformedVertices() {return boxData.transformedVertices;}

    RigidBodyType bodyType = RB_DYNAMIC;
    RigidBodyShape bodyShape = RB_BOX;

    float Radius() const {return circleData.radius;}

    float gravityScale = 1.0f;

    //TODO: Make this offset the center!!
    glm::vec2 centerOfMass = glm::vec2(0);

    //Make sure this sucker is assigned
    LilObj<Transform> transform;

    bool IsTrigger() const
    {
        return isTrigger;
    }

    void SetIsTrigger(bool isTrigger)
    {
        if (transform.Exists())
        {
            if (!isTrigger)
            {
                if (transform->getIsChild())
                {
                    std::cout << "Child objects cannot have non-trigger RigidBodies." << std::endl;
                    return;
                }
            }
            this->isTrigger = isTrigger;
        }
    }

private:

    friend class PhysicsSystem;
    friend class PhysicsEventHandler;
    static inline float EPSILON = 0.08f;

    int collisionTag = 0;

    bool isTrigger = false;

    bool isSleeping = false;
    float linearDamping = 0.98f;
    float angularDamping = 0.98f;

    glm::vec2 linearVelocity = {};
    float angularVelocity = 0.0f;
    glm::vec2 accumulatedForce = {};

    float inertia = 0.0f;
    float invInertia = 0.0f;
    float CalculateRotationalInertia() const;

    float density = 1.0f;
    float mass = 1.0f;
    float invMass = 1.0f;
    void CalcInvMass();

    glm::vec2 linearConstraints = glm::vec2(1.0f);
    float angularConstraint = 1.0f;

    AABB aabb;
    //Hook up to gameobject my guy.

    int numVertices = 0;

    PhysicsMaterial material;
    BoxData boxData;
    CircleData circleData;

    static glm::vec2 transformVertex(glm::vec2 v, glm::vec2 tr, float r);
};



#endif //RIGIDBODY_H
