//
// Created by Somed on 12/30/2024.
//

#include "RenderOrder.h"
#include "Sprite.h"
#include "EngineStuffs/GameObject.h"

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
        if (spritePool->poolDir[i]->getLayer() > ly)
        {
            ly++;
            layerIndices.push_back(i);
        }
    }
    layerIndices.push_back(spritePool->activeLine);
}


void RenderOrder::OrderByAxis()
{
    if (axis == glm::vec3(0))
    {
        return;
    }

    std::vector<Sprite*> sortedSprites = spritePool->poolDir;

    //Seriously try and make this more efficient!
    for (int i = 0; i < layerIndices.size() - 1; i++)
    {
        //Get help for this one...
        //std::sort(sortedSprites.begin() + layerIndices[i], sortedSprites.end() + (layerIndices[i + 1] - 1), compareAxis);
    }

    for (int i = 0; i < spritePool->activeLine; i++)
    {
        spritePool->SwapObjects(spritePool->poolDir[i], sortedSprites[i]);
    }
}

bool RenderOrder::compareAxis(Sprite* const &spr1, Sprite* const &spr2)
{
    glm::vec3 pos1 = spr1->getObject()->transform.GlobalPosition();
    glm::vec3 pos2 = spr2->getObject()->transform.GlobalPosition();
    float dist = glm::dot(axis, pos2) - glm::dot(axis, pos1);
    return dist > 0;
}



