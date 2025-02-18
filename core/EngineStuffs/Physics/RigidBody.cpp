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

void RigidBody::Integrate(float deltaTime)
{
    transform->Translate(linearVelocity * deltaTime);
    transform->Rotate(angularVelocity * deltaTime);
}






