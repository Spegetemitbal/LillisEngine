//
// Created by Somed on 2/15/2025.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "EngineStuffs/Component.h"
#include "EngineStuffs/Transform.h"

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

    void UpdateVertices();

    void Integrate(float deltaTime);

    inline static const int triangles[6] = {0,1,2,0,2,3};
    glm::vec2* GetTransformedVertices() {return transformedVertices;}

    RigidBodyType bodyType = RB_DYNAMIC;
    RigidBodyShape bodyShape = RB_BOX;

    //Make sure this sucker is assigned
    LilObj<Transform> transform;

private:
    glm::vec2 linearVelocity = glm::vec3(0);
    float angularVelocity = 0.0f;

    float density = 1.0f;
    float mass = 1.0f;
    //Bounciness, between 0 and 1
    float restitution = 0;
    float area = 1;

    glm::vec2 vertices[4];
    glm::vec2 transformedVertices[4];
    //Hook up to gameobject my guy.
    bool transformFlag = true;

    float radius = 1;
    glm::vec2 size = glm::vec2(1.0f);

    static glm::vec2 transformVertex(glm::vec2 v, glm::vec2 tr, float r);
};



#endif //RIGIDBODY_H
