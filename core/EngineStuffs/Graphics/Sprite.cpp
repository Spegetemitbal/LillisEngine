//
// Created by Somed on 12/30/2024.
//

#include "Sprite.h"

#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/WorldManager.h"
#include "EngineStuffs/MemoryManagement/LilObj.h"

Sprite::Sprite(const std::string& spr): aabb(0,0,0,0)
{
    image = spr;
}

Sprite::Sprite(): aabb(0,0,0,0)
{
}


void Sprite::SetLayer(unsigned int lyr)
{
    layer = lyr;
    WorldManager::getInstance()->GetCurrentWorld()->setSpriteLayer(this);
}

glm::vec2 Sprite::getRenderCenter()
{
    glm::vec2 pos = getRenderLocation();
    return {pos.x + (renderSize.x / 2), pos.y + (renderSize.y / 2)};
}

glm::vec2 Sprite::getRenderLocation()
{
    LazySetTransform();
    return transform->GlobalPosition() + offset - (renderSize * 0.5f);
}

float Sprite::getRenderRotation()
{
    LazySetTransform();
    return transform->GlobalRotation();
}

glm::vec2 Sprite::getRenderScale()
{
    LazySetTransform();
    return transform->GlobalScale();
}

void Sprite::LazySetTransform()
{
    if (!transform.Exists())
    {
        transform = thisObject->transform;
    }
}

AABB Sprite::getAABB()
{
    LazySetTransform();

    //TODO: Cache this
    glm::vec2 transformedVertices[4];

    transformedVertices[0] = transformVertex(verts[0], transform->GlobalPosition() + offset, transform->GlobalRotation());
    transformedVertices[1] = transformVertex(verts[1], transform->GlobalPosition() + offset, transform->GlobalRotation());
    transformedVertices[2] = transformVertex(verts[2], transform->GlobalPosition() + offset, transform->GlobalRotation());
    transformedVertices[3] = transformVertex(verts[3], transform->GlobalPosition() + offset, transform->GlobalRotation());

    aabb.UpdateBox(transformedVertices);

    return aabb;
}

void Sprite::InitVertices()
{
    float left = -renderSize.x / 2.0f;
    float right = left + renderSize.x;
    float bottom = -renderSize.y / 2.0f;
    float top = bottom + renderSize.y;

    verts[0] = {left, top};
    verts[1] = {right, top};
    verts[2] = {right, bottom};
    verts[3] = {left, bottom};
}

glm::vec2 Sprite::transformVertex(glm::vec2 v, glm::vec2 tr, float r)
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





