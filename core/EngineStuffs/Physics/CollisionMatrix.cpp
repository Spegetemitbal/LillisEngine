//
// Created by Somed on 7/27/2025.
//

#include "CollisionMatrix.h"

void CollisionMatrix::AddCollisionExclusion(int layer1, int layer2)
{
    collisionMatrix.insert({layer1, layer2});
}

bool CollisionMatrix::GetCollisionExclusion(int layer1, int layer2)
{
    if (collisionMatrix.empty())
    {
        return false;
    }
    GridHash gh = GridHash(layer1, layer2);
    GridHash hg = GridHash(layer2, layer1);
    return collisionMatrix.contains(gh) || collisionMatrix.contains(hg);
}

void CollisionMatrix::RemoveCollisionExclusion(int layer1, int layer2)
{
    if (collisionMatrix.empty())
    {
        return;
    }
    GridHash gh = GridHash(layer1, layer2);
    GridHash hg = GridHash(layer2, layer1);
    if (collisionMatrix.contains(gh))
    {
        collisionMatrix.erase(gh);
    }
    if (collisionMatrix.contains(hg))
    {
        collisionMatrix.erase(hg);
    }
}


