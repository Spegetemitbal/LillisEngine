//
// Created by Somed on 12/30/2024.
//

#include "Sprite.h"

#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/GameObjectManager.h"
#include "EngineStuffs/MemoryManagement/LilObj.h"

Sprite::Sprite(const std::string& spr)
{
    image = spr;
}

void Sprite::SetLayer(unsigned int lyr)
{
    layer = lyr;
    GameObjectManager::world->setSpriteLayer(this);
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



