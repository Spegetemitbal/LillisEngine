//
// Created by Somed on 12/30/2024.
//

#include "RenderOrder.h"
#include "Sprite.h"

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
}

