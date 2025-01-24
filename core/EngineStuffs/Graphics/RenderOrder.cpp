//
// Created by Somed on 12/30/2024.
//

#include "RenderOrder.h"
#include "Sprite.h"
#include "EngineStuffs/GameObject.h"

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
        if (spritePool->poolDir[i]->getLayer() > ly)
        {
            ly++;
            layerIndices.push_back(i);
        }
    }
    layerIndices.push_back(spritePool->activeLine);

    OrderByAxis({spr->thisObject.GetID()});
}

//Assumes an ordered setup.
void RenderOrder::OrderByAxis(const std::unordered_set<unsigned int>& toUpdate)
{
    if (axis == glm::vec3(0) || !sorted)
    {
        return;
    }

    if (toUpdate.size() / spritePool->poolDir.size() > 0.75)
    {
        OrderAll();
        return;
    }

    for (int i = 0; i < layerIndices.size() - 1; i++)
    {
        if (layerIndices[i] < layerIndices[i + 1] + 1)
        {
            SingleSort(layerIndices[i], layerIndices[i + 1] - 1, toUpdate);
        }
    }
}

//Assumes an entirely unordered setup.
void RenderOrder::OrderAll()
{
    //Sort once for each sorting layer!
    for (int i = 0; i < layerIndices.size() - 1; i++)
    {
        if (layerIndices[i] < layerIndices[i + 1] + 1)
        {
            RenderSort(layerIndices[i], layerIndices[i + 1] - 1);
        }
    }
    sorted = true;
}

//If spr1 is further along the axis than spr2 (rendered later), dist will be negative. Return 1
int RenderOrder::compareAxis(Sprite* const &spr1, Sprite* const &spr2)
{
    glm::vec3 pos1 = spr1->getObject()->transform.GlobalPosition();
    glm::vec3 pos2 = spr2->getObject()->transform.GlobalPosition();
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

int RenderOrder::Partition(int low, int high)
{
    // Selecting last element as the pivot
    Sprite* pivot = spritePool->poolDir[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        //If 1 is further along, send it back
        if (compareAxis(spritePool->poolDir[j], pivot) >= 0)
        {
            i++;
            spritePool->SwapObjects(spritePool->poolDir[i], spritePool->poolDir[j]);
        }
    }

    // Put pivot to its position
    spritePool->SwapObjects(spritePool->poolDir[i + 1], spritePool->poolDir[high]);

    // Return the point of partition
    return (i + 1);
}

void RenderOrder::RenderSort(int low, int high)
{
    if (low < high) {

        int pi = Partition(low, high);

        RenderSort(low, pi - 1);
        RenderSort(pi + 1, high);
    }
}

//For each marked item in need of sorting, moves to correct spot while ignoring other marked items.
//Then unmarks itself.
void RenderOrder::SingleSort(int low, int high, std::unordered_set<unsigned int> toUpdate)
{
    for (int i = low; i <= high; i++)
    {
        //Check if item is marked.
        if (toUpdate.contains(spritePool->poolDir[i]->entityID))
        {
            bool goingLeft = false, goingRight = false;
            int viableLeft = -1, viableRight = -1;

            //First check if there are any unmarked things in each direction.
            for (int j = i + 1; j <= high; j++)
            {
                if (!toUpdate.contains(spritePool->poolDir[j]->entityID))
                {
                    viableRight = j;
                    break;
                }
            }
            for (int j = i - 1; j >= low; j--)
            {
                if (!toUpdate.contains(spritePool->poolDir[j]->entityID))
                {
                    viableLeft = j;
                    break;
                }
            }

            if (viableLeft >= 0 || viableRight >= 0)
            {
                //Determine direction
                if (viableLeft >= 0)
                {
                    goingLeft = compareAxis(spritePool->poolDir[i], spritePool->poolDir[viableLeft]) == 1;
                }
                if (viableRight >= 0)
                {
                    goingRight = compareAxis(spritePool->poolDir[i], spritePool->poolDir[viableRight]) == -1;
                }

                //Scoot in that direction
                int toMove = i;
                int nextMove;
                if (goingLeft)
                {
                    nextMove = viableLeft;
                    while (nextMove != toMove)
                    {
                        //Determine if the next place to move is out of order.
                        if (compareAxis(spritePool->poolDir[toMove], spritePool->poolDir[nextMove]) == 1)
                        {
                            spritePool->SwapObjects(spritePool->poolDir[toMove], spritePool->poolDir[nextMove]);
                            toMove = nextMove;
                        } else
                        {
                            break;
                        }

                        //Find next viable place
                        int nextCheck = nextMove;
                        while (nextCheck >= low)
                        {
                            nextCheck--;
                            if (!toUpdate.contains(spritePool->poolDir[nextCheck]->entityID))
                            {
                                nextMove = nextCheck;
                                break;
                            }
                        }
                    }
                } else if (goingRight)
                {
                    nextMove = viableRight;
                    while (nextMove != toMove)
                    {
                        //Determine if the next place to move is out of order.
                        if (compareAxis(spritePool->poolDir[toMove], spritePool->poolDir[nextMove]) == -1)
                        {
                            spritePool->SwapObjects(spritePool->poolDir[toMove], spritePool->poolDir[nextMove]);
                            toMove = nextMove;
                        } else
                        {
                            break;
                        }

                        //Find next viable place
                        int nextCheck = nextMove;
                        while (nextCheck <= high)
                        {
                            nextCheck++;
                            if (!toUpdate.contains(spritePool->poolDir[nextCheck]->entityID))
                            {
                                nextMove = nextCheck;
                                break;
                            }
                        }
                    }
                }
            }

            toUpdate.erase(spritePool->poolDir[i]->entityID);
        }
    }
}





