//
// Created by Somed on 12/30/2024.
//

#include "Sprite.h"

#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/GameObjectManager.h"

Sprite::Sprite(const std::string& spr)
{
    image = spr;
}

void Sprite::SetLayer(unsigned int lyr)
{
    layer = lyr;
    GameObjectManager::world->setSpriteLayer(this);
}


glm::vec2 Sprite::getRenderLocation()
{
    glm::vec3 tr = thisObject->transform.GlobalPosition();
    return {tr.x + offset.x, tr.y + offset.y};
}

unsigned int Sprite::getRenderRotation()
{
    return thisObject->transform.GlobalRotation();
}

glm::vec2 Sprite::getRenderScale()
{
    return thisObject->transform.GlobalScale();
}


