//
// Created by Somed on 12/30/2024.
//

#include "RenderOrder.h"
#include "Sprite.h"
#include "EngineStuffs/GameObject.h"
#include "../Tilemaps/TileMap.h"
#include "../Particles/ParticleEmitter.h"

//Moves sprite to proper layer region!
void RenderOrder::MoveSprite(LilObj<Sprite> spr)
{
    unsigned int layer = spr->getLayer();

    if (layer > highestLayer)
    {
        highestLayer = layer;
    }
    if (highestLayer >= maxLayers)
    {
        SetRenderAxis(false);
    }
}

//Get Crackin.
void RenderOrder::CalculateOrder(std::vector<Sprite*>& spritesOnScreen, const float& up, const float& down)
{
    const float dist = up - down;
    const float invDist = 1 / dist;
    const float planeDist = FAR_PLANE - NEAR_PLANE;

    float layerDif;

    if (highestLayer == -1)
    {
        layerDif = 0;
    } else
    {
        layerDif = planeDist / ((float)highestLayer + 1);
    }

    for (auto spr : spritesOnScreen)
    {
        spr->renderValue = FAR_PLANE - (layerDif * (float)spr->layer);
        if (axis != glm::vec2(0))
        {
            //Basically just inverse LERP.
            float toAdd = (up - spr->getRenderLocation().y) * invDist;
            if (layerDif > 0)
            {
                toAdd *= layerDif;
            } else
            {
                toAdd *= planeDist;
            }
            toAdd *= AXIS_BUFFER;
            spr->renderValue -= toAdd;
        }
    }
}

void RenderOrder::CalculateOrder(std::vector<ParticleEmitter*> &emitters)
{
    const float planeDist = FAR_PLANE - NEAR_PLANE;

    float layerDif;

    if (highestLayer == -1)
    {
        return;
    } else
    {
        layerDif = planeDist / ((float)highestLayer + 1);
    }

    for (auto emitter : emitters)
    {
        emitter->layer = std::clamp(emitter->layer, (unsigned int)0, highestLayer);
        if (axis != glm::vec2(0))
        {
            emitter->SetDepthData(FAR_PLANE - (layerDif * (float)emitter->layer), layerDif);
        } else
        {
            emitter->SetDepthData(FAR_PLANE - (layerDif * (float)emitter->layer), 0);
        }
    }
}


void RenderOrder::CalculateTileOrder(TileMap *tileMap, const float &up, const float &down)
{
    const float dist = up - down;
    const float invDist = 1 / dist;
    const float planeDist = FAR_PLANE - NEAR_PLANE;

    const unsigned int layer = tileMap->GetLayer();

    if (layer > highestLayer)
    {
        highestLayer = layer;
    }

    float layerDif;

    if (highestLayer == -1)
    {
        layerDif = 0;
    } else
    {
        layerDif = planeDist / ((float)highestLayer + 1);
    }

    for (auto & til : tileMap->tilesToRender)
    {
        til.zVal = FAR_PLANE - (layerDif * (float)layer);
        if (axis != glm::vec2(0))
        {
            //Basically just inverse LERP.
            float toAdd = (up - til.worldPos.y) * invDist;
            if (layerDif > 0)
            {
                toAdd *= layerDif;
            } else
            {
                toAdd *= planeDist;
            }
            toAdd *= AXIS_BUFFER;
            til.zVal -= toAdd;
        }
    }
}


//If spr1 is further along the axis than spr2 (rendered later), dist will be negative. Return 1
int RenderOrder::compareAxis(Sprite* const &spr1, Sprite* const &spr2)
{
    glm::vec2 pos1 = spr1->getObject()->transform->GlobalPosition();
    glm::vec2 pos2 = spr2->getObject()->transform->GlobalPosition();
    float dist1 = glm::dot(axis, pos1);
    float dist2 = glm::dot(axis, pos2);
    if (dist1 > dist2)
    {
        return 1;
    } else if (dist1 < dist2)
    {
        return -1;
    }
    return 0;
}





