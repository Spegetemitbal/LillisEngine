//
// Created by Somed on 6/12/2025.
//
#include "SpatialHasher.h"

void SpatialHasher::SetTileSize(float sz)
{
    tileSize = std::clamp(sz, 5.0f, 100.0f);
    invTileSize = 1.0f / tileSize;
    keys.clear();
    spatialHash.clear();
}

std::pair<std::multimap<GridHash, int>::iterator, std::multimap<GridHash, int>::iterator> SpatialHasher::GetObjectsInGrid(GridHash g)
{
    return spatialHash.equal_range(g);
}

std::pair<std::multimap<GridHash, std::pair<int, int> >::iterator, std::multimap<GridHash, std::pair<int, int> >::iterator> SpatialHasher::GetTilesInGrid(GridHash g)
{
    return tileHash.equal_range(g);
}


void SpatialHasher::InsertObject(AABB aabb, int iter)
{
    int minX, maxX, minY, maxY;
    glm::vec2 minPoint = aabb.min * invTileSize;
    glm::vec2 maxPoint = aabb.max * invTileSize;
    minX = (int)std::round(minPoint.x);
    minY = (int)std::round(minPoint.y);
    maxX = (int)std::round(maxPoint.x);
    maxY = (int)std::round(maxPoint.y);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            GridHash g = GridHash(x, y);
            spatialHash.emplace(g, iter);
            if (!keys.contains(g))
            {
                keys.insert(g);
            }
        }
    }
}

void SpatialHasher::InsertTile(AABB aabb, int xIter, int yIter)
{
    int minX, maxX, minY, maxY;
    glm::vec2 minPoint = aabb.min * invTileSize;
    glm::vec2 maxPoint = aabb.max * invTileSize;
    minX = (int)std::round(minPoint.x);
    minY = (int)std::round(minPoint.y);
    maxX = (int)std::round(maxPoint.x);
    maxY = (int)std::round(maxPoint.y);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            GridHash g = GridHash(x, y);
            tileHash.emplace(g, std::make_pair(xIter, yIter));
        }
    }
}

void SpatialHasher::DelTile(AABB aabb, int xIter, int yIter)
{
    int minX, maxX, minY, maxY;
    glm::vec2 minPoint = aabb.min * invTileSize;
    glm::vec2 maxPoint = aabb.max * invTileSize;
    minX = (int)std::round(minPoint.x);
    minY = (int)std::round(minPoint.y);
    maxX = (int)std::round(maxPoint.x);
    maxY = (int)std::round(maxPoint.y);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            GridHash g = GridHash(x, y);
            auto ret = GetTilesInGrid(g);
            auto iter = ret.first;
            for (; iter != ret.second; ++iter)
            {
                if (iter->second.first == xIter && iter->second.second == yIter)
                {
                    tileHash.erase(iter);
                    break;
                }
            }
        }
    }
}


void SpatialHasher::Clear()
{
    spatialHash.clear();
    keys.clear();
}

void SpatialHasher::ClearTiles()
{
    tileHash.clear();
}



