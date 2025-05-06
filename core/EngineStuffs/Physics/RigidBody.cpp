//
// Created by Somed on 2/15/2025.
//

#include "RigidBody.h"
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/Transform.h"

RigidBody::RigidBody(): aabb(1,1,1,1)
{
    //Scoodledoot
}


void RigidBody::SetMass(float mass)
{
    this->mass = mass;
    if (mass < 0.0f)
    {
        this->mass = 0.0f;
    }
    CalcInvMass();
    inertia = CalculateRotationalInertia();
    if (bodyType == RigidBodyType::RB_STATIC)
    {
        invMass = 0.0f;
        invInertia = 0.0f;
    } else
    {
        invInertia = 1.0f/inertia;
    }
}

void RigidBody::SetDensity(float density)
{
    if (density > 0.5f)
    {
        this->density = density;
    } else
    {
        this->density = 0.5f;
    }
}

void RigidBody::SetRestitution(float restitution)
{
    if (restitution > 1)
    {
        this->material.restitution = 1;
    } else if (restitution < 0)
    {
        this->material.restitution = 0;
    } else
    {
        this->material.restitution = restitution;
    }
}

void RigidBody::SetSize(glm::vec2 newSize)
{
    if (newSize.x <= 1)
    {
        newSize.x = 1.0f;
    }

    if (newSize.y <= 1)
    {
        newSize.y = 1.0f;
    }

    this->boxData.boxSize = newSize;
    InitVertices();
}

void RigidBody::SetDynamicFriction(float fric)
{
    material.dynamicFriction = std::clamp(fric, 0.0f, 1.0f);
}

void RigidBody::SetStaticFriction(float fric)
{
    material.staticFriction = std::clamp(fric, 0.0f, 1.0f);
}




void RigidBody::InitVertices()
{
    if (this->bodyShape == RigidBodyShape::RB_BOX)
    {
        float left = -boxData.boxSize.x / 2.0f;
        float right = left + boxData.boxSize.x;
        float bottom = -boxData.boxSize.y / 2.0f;
        float top = bottom + boxData.boxSize.y;

        boxData.vertices[0] = {left, top};
        boxData.vertices[1] = {right, top};
        boxData.vertices[2] = {right, bottom};
        boxData.vertices[3] = {left, bottom};
        numVertices = 4;
        //UpdateVertices();
    }
}


glm::vec2 RigidBody::transformVertex(glm::vec2 v, glm::vec2 tr, float r)
{
    //You know this is gonna be your own transform-

    r = glm::radians(r);

    float Sin = glm::sin(r);
    float Cos = glm::cos(r);

    return {
        Cos * v.x - Sin * v.y + tr.x,
        Sin * v.x + Cos * v.y + tr.y
        };
}

void RigidBody::UpdateVertices()
{
    boxData.transformedVertices[0] = transformVertex(boxData.vertices[0], transform->GlobalPosition(), transform->GlobalRotation());
    boxData.transformedVertices[1] = transformVertex(boxData.vertices[1], transform->GlobalPosition(), transform->GlobalRotation());
    boxData.transformedVertices[2] = transformVertex(boxData.vertices[2], transform->GlobalPosition(), transform->GlobalRotation());
    boxData.transformedVertices[3] = transformVertex(boxData.vertices[3], transform->GlobalPosition(), transform->GlobalRotation());
    //GetAABB();
}

void RigidBody::Integrate(float deltaTime, glm::vec2 gravity)
{
    if (bodyType == RigidBodyType::RB_STATIC)
    {
        linearVelocity = {};
        angularVelocity = 0.0f;
        return;
    }

    glm::vec2 k1,k2,k3,k4;

    glm::vec2 accumulatedForce = gravity;

    //RUNGE KUTTA DEEZ NUTS.

    //Compute k1
    //TODO: Replace with accumulated force.
    //TODO: technically all accumulatedforce things are supposed to be multiplied by invMass. But currently that's too slow.
    glm::vec2 acceleration = accumulatedForce;
    k1 = acceleration * deltaTime;

    //Compute k2
    acceleration = accumulatedForce + (k1 * 0.5f);
    k2 = acceleration * deltaTime;

    //Compute k3
    acceleration = accumulatedForce + (k2 * 0.5f);
    k3 = acceleration * deltaTime;

    //Compute k4
    acceleration = accumulatedForce + (k3 * 0.5f);
    k4 = acceleration * deltaTime;

    // (((k2 * 2) + k1) + (k3 * 2) + k4)) / 6;
    glm::vec2 deltaVelocity = (k1 + (k2 * 2.0f) + (k3 * 2.0f) + k4) / 6.0f;
    linearVelocity += deltaVelocity;
    glm::vec2 deltaPosition = linearVelocity * deltaTime;
    transform->Translate(deltaPosition);

    linearVelocity *= linearDamping;
    angularVelocity *= angularDamping;

    if (glm::length(linearVelocity) <= EPSILON)
    {
        linearVelocity = {};
        isSleeping = true;
        return;
    }

    //transform->Rotate((angularVelocity) * deltaTime);

   // accumulatedForce = {0,0};
    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        if (transform->getToUpdate())
        {
            UpdateVertices();
        }
    }
}

void RigidBody::CalcInvMass()
{
    if (mass > 0.0f)
    {
        invMass = 1.0f / mass;
    } else
    {
        invMass = 0.0f;
    }
}

AABB RigidBody::GetAABB()
{
    if (!transform->getToUpdate())
    {
        //TODO: ignore this loop if you change shape.
        //Ignores invalid initialization.
        if (aabb.min != aabb.max)
        {
            return aabb;
        }
    }

    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        for (int i = 0; i < 4; i++)
        {
            //For polygons, this is the same thing dude.
            glm::vec2 v = boxData.transformedVertices[i];
            if (v.x < minX) {minX = v.x;}
            if (v.x > maxX) {maxX = v.x;}
            if (v.y < minY) {minY = v.y;}
            if (v.y > maxY) {maxY = v.y;}
        }
    } else if (bodyShape == RigidBodyShape::RB_CIRCLE)
    {
        minX = transform->GlobalPosition().x - circleData.radius;
        minY = transform->GlobalPosition().y - circleData.radius;
        maxX = transform->GlobalPosition().x + circleData.radius;
        maxY = transform->GlobalPosition().y + circleData.radius;
    } else
    {
        throw std::runtime_error("Invalid body shape");
    }

    aabb = {minX, minY, maxX, maxY};
    return aabb;
}

float RigidBody::CalculateRotationalInertia() const
{

    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        return (1.0f / 12.0f) * mass * (boxData.boxSize.x * boxData.boxSize.x + boxData.boxSize.y * boxData.boxSize.y);
    } else if (bodyShape == RigidBodyShape::RB_CIRCLE)
    {
        return 0.5f * mass * circleData.radius * circleData.radius;
    } else
    {
        std::cout << "Invalid shape" << std::endl;
        return 0;
    }
}

void RigidBody::AddForce(glm::vec2 force)
{
    accumulatedForce += force;
}

void RigidBody::AddImpulse(glm::vec2 impulse, float torque)
{
    linearVelocity += impulse;
    angularVelocity += torque;
    if (glm::length(impulse) > EPSILON && torque > EPSILON)
    {
        isSleeping = false;
    }
}

void RigidBody::SetForce(glm::vec2 force)
{
    accumulatedForce = force;
}










