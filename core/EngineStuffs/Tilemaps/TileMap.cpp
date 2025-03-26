//
// Created by Somed on 3/17/2025.
//

#include "TileMap.h"

#include <utility>

void TileMap::setTile(std::pair<int, int> index, char tile)
{
    if (!tileSet.tileSet.empty())
    {
        int newTile = -1;
        int tileIndex = 0;
        if (tileSet.inputConversion.contains(tile))
        {
            newTile = tileSet.inputConversion[tile];
            if (index.first >= dimensions.first || index.second >= dimensions.second || index.first < 0 || index.second < 0)
            {
                std::cout << "Tile index out of bounds" << std::endl;
                return;
            }
            //Make sure this doesn't go out of bounds. Test!
            tileIndex = index.first + (index.second * dimensions.first);
        }
        tiles[tileIndex] = newTile;
    } else
    {
        throw;
    }
}

TileMap::TileMap()
{
    tileGrid = nullptr;
}

TileMap::TileMap(TileGrid *grid, TileSet tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions)
{
    //For square, bottom left is 0,0
    //For isometric, left center is 0,0
    //For hexagon... well that's not implemented dude.

    tileGrid = grid;
    this->tileSet = std::move(tileSet);
    this->gridIndex = gridIndex;
    this->dimensions = dimensions;

    int dimSize = dimensions.first * dimensions.second;

    tiles.reserve(dimSize);
    tileWorldPositions.reserve(dimSize);
    for (int y = 0; y < dimensions.second; y += dimensions.first)
    {
        for (int x = 0; x < dimensions.first; x++)
        {
            tileWorldPositions.emplace_back(tileGrid->GridToWorldSpace({x + gridIndex.first,y + gridIndex.second}));
            tiles.push_back(-1);
        }
    }
    tilesToRender.reserve(dimSize);
}

glm::vec2 TileMap::CullMap(AABB camAABB)
{
    //BROAD PHASE

    //TODO partitioning!

    //NARROW PHASE

    tilesToRender.clear();

    float minTileY = std::numeric_limits<float>::infinity();
    float maxTileY = -std::numeric_limits<float>::infinity();

    //Half tile
    glm::vec2 tileSize = tileGrid->GetTileSize() * 0.5f;

    AABB tileAABB = {0,0,0,0};

    for (int i = 0; i < tiles.size(); i++)
    {
        if (tiles[i] != -1)
        {
            tileAABB.min = tileWorldPositions[i] - tileSize;
            tileAABB.max = tileWorldPositions[i] + tileSize;
            if (AABB::Intersect(camAABB, tileAABB))
            {
                tilesToRender.emplace_back(tileWorldPositions[i], tiles[i], 0);
                if (tileWorldPositions[i].y < minTileY)
                {
                    minTileY = tileWorldPositions[i].y;
                }
                if (tileWorldPositions[i].y > maxTileY)
                {
                    maxTileY = tileWorldPositions[i].y;
                }
            }
        }
    }

    return {minTileY, maxTileY};
}





