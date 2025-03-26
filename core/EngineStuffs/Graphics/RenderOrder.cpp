//
// Created by Somed on 12/30/2024.
//

#include "RenderOrder.h"
#include "Sprite.h"
#include "EngineStuffs/GameObject.h"
#include "../Tilemaps/TileMap.h"

//Moves sprite to proper layer region!
void RenderOrder::MoveSprite(LilObj<Sprite> spr)
{
    unsigned int layer = spr->getLayer();
    unsigned int EOP = spritePool->activeLine - 1;
    unsigned int sprIndex = spritePool->FindObjectIndex(spritePool->GetObjByID<Sprite>(spr.GetID()));

    if (sprIndex == 0)
    {
        if (EOP == 0)
        {
            return;
        }
        while (sprIndex < EOP)
        {
            if (layer > spritePool->poolDir[sprIndex + 1]->getLayer())
            {
                spritePool->SwapObjects(spritePool->poolDir[sprIndex], spritePool->poolDir[sprIndex + 1]);
                sprIndex++;
            } else
            {
                break;
            }
        }
    } else if (sprIndex == EOP)
    {
        while (sprIndex > 0)
        {
            if (layer < spritePool->poolDir[sprIndex - 1]->getLayer())
            {
                spritePool->SwapObjects(spritePool->poolDir[sprIndex], spritePool->poolDir[sprIndex - 1]);
                sprIndex--;
            } else
            {
                break;
            }
        }
    } else
    {
        bool goingLeft = true;
        if (layer > spritePool->poolDir[sprIndex + 1]->getLayer())
        {
            goingLeft = false;
        }

        if (goingLeft)
        {
            while (sprIndex > 0)
            {
                if (layer < spritePool->poolDir[sprIndex - 1]->getLayer())
                {
                    spritePool->SwapObjects(spritePool->poolDir[sprIndex], spritePool->poolDir[sprIndex - 1]);
                    sprIndex--;
                } else
                {
                    break;
                }
            }
        } else
        {
            while (sprIndex < EOP)
            {
                if (layer > spritePool->poolDir[sprIndex + 1]->getLayer())
                {
                    spritePool->SwapObjects(spritePool->poolDir[sprIndex], spritePool->poolDir[sprIndex + 1]);
                    sprIndex++;
                } else
                {
                    break;
                }
            }
        }
    }

    //Determines where each layer group begins
    layerIndices.clear();
    int ly = -1;
    for (int i = 0; i < spritePool->activeLine; i++)
    {
        int sprLayer = spritePool->poolDir[i]->getLayer();
        if (sprLayer > ly)
        {
            ly++;
            layerIndices.push_back(i);
        }
    }
    highestLayer = ly;
    if (highestLayer >= maxLayers)
    {
        SetRenderAxis(false);
    }
    layerIndices.push_back(spritePool->activeLine);
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

void RenderOrder::CalculateTileOrder(TileMap *tileMap, const float &up, const float &down)
{
    const float dist = up - down;
    const float invDist = 1 / dist;
    const float planeDist = FAR_PLANE - NEAR_PLANE;

    const unsigned int layer = tileMap->layer;

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

    for (auto til : tileMap->tilesToRender)
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





