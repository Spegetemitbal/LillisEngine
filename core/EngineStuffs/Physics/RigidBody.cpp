//
// Created by Somed on 2/15/2025.
//

#include "RigidBody.h"
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/Transform.h"

void RigidBody::SetMass(float mass)
{
    if (mass > 0)
    {
        this->mass = mass;
    } else
    {
        this->mass = 1;
    }
    CalcInvMass();
    inertia = CalculateRotationalInertia();
    invInertia = 1.0f/inertia;
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
        this->restitution = 1;
    } else if (restitution < 0)
    {
        this->restitution = 0;
    } else
    {
        this->restitution = restitution;
    }
}

void RigidBody::SetSize(glm::vec2 size)
{
    this->size = size;
    InitVertices();
}


void RigidBody::InitVertices()
{
    if (this->bodyShape == RigidBodyShape::RB_BOX)
    {
        float left = -size.x / 2.0f;
        float right = left + size.x;
        float bottom = -size.y / 2.0f;
        float top = bottom + size.y;

        vertices[0] = {left, top};
        vertices[1] = {right, top};
        vertices[2] = {right, bottom};
        vertices[3] = {left, bottom};
    }
}


glm::vec2 RigidBody::transformVertex(glm::vec2 v, glm::vec2 tr, float r)
{
    //You know this is gonna be your own transform-

    float Sin = glm::sin(r);
    float Cos = glm::cos(r);

    return {
        Cos * v.x - Sin * v.y + tr.x,
        Sin * v.x + Cos * v.y + tr.y
        };
}

void RigidBody::UpdateVertices()
{
    transformedVertices[0] = transformVertex(vertices[0], transform->GlobalPosition(), transform->GlobalRotation());
    transformedVertices[1] = transformVertex(vertices[1], transform->GlobalPosition(), transform->GlobalRotation());
    transformedVertices[2] = transformVertex(vertices[2], transform->GlobalPosition(), transform->GlobalRotation());
    transformedVertices[3] = transformVertex(vertices[3], transform->GlobalPosition(), transform->GlobalRotation());
}

void RigidBody::Integrate(float deltaTime, glm::vec2 gravity)
{
    accumulatedForce += gravity * deltaTime * gravityScale;

    glm::vec2 linearAcceleration = accumulatedForce / mass;

    linearVelocity += linearAcceleration * deltaTime;

    transform->Translate(linearVelocity * deltaTime);
    transform->Rotate(angularVelocity * deltaTime);

    accumulatedForce = {0,0};
    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        if (transform->getToUpdate())
        {
            UpdateVertices();
            GetAABB();
        }
    }
}

void RigidBody::CalcInvMass()
{
    if (bodyType != RigidBodyType::RB_DYNAMIC)
    {
        invMass = 1.0f / mass;
    } else
    {
        invMass = 0.0f;
    }
}

AABB RigidBody::GetAABB()
{
    if (transform->getToUpdate())
    {
        //TODO: ignore this loop if you change shape.
        return aabb;
    }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();

    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        for (int i = 0; i < 4; i++)
        {
            //For polygons, this is the same thing dude.
            glm::vec2 v = transformedVertices[i];
            if (v.x < minX) {minX = v.x;}
            if (v.x > maxX) {maxX = v.x;}
            if (v.y < minX) {minX = v.y;}
            if (v.y > maxX) {maxX = v.y;}
        }
    } else if (bodyShape == RigidBodyShape::RB_CIRCLE)
    {
        minX = transform->GlobalPosition().x - radius;
        minY = transform->GlobalPosition().y - radius;
        maxX = transform->GlobalPosition().x + radius;
        maxY = transform->GlobalPosition().y + radius;
    } else
    {
        throw std::runtime_error("Invalid body shape");
    }

    return {minX, minY, maxX, maxY};
}

float RigidBody::CalculateRotationalInertia()
{
    if (bodyShape == RigidBodyShape::RB_BOX)
    {
        return (1.0f / 12.0f) * mass * (size.x * size.x + size.y * size.y);
    } else if (bodyShape == RigidBodyShape::RB_CIRCLE)
    {

    } else
    {
        std::cout << "Invalid shape" << std::endl;
        return 0;
    }
}







