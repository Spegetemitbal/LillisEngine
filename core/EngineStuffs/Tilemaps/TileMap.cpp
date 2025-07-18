//
// Created by Somed on 3/17/2025.
//

#include "TileMap.h"

#include <utility>

void TileMap::setTile(std::pair<int, int> index, char tile, bool reloadColliders)
{
    if (!tileSet.tileSet.empty())
    {
        int newTile = -1;
        int tileIndex = FindIndexOfTile(index.first, index.second);
        if (tileSet.inputConversion.contains(tile))
        {
            newTile = tileSet.inputConversion[tile];
            bool changeCol = tileHasCollider[tileIndex];
            tileHasCollider[tileIndex] = tileSet.tileSet[tileSet.inputConversion[tile]].hasCollider;
            if (changeCol != tileHasCollider[tileIndex] && reloadColliders)
            {
                GenerateColliders();
            }
            if (index.first >= dimensions.first || index.second >= dimensions.second || index.first < 0 || index.second < 0)
            {
                std::cout << "Tile index out of bounds" << std::endl;
                return;
            }
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

TileMap::TileMap(TileGrid *grid, TileSet tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions, unsigned int layer, unsigned int chunkSize)
{
    //For square, bottom left is 0,0
    //For isometric, left center is 0,0
    //For hexagon... well that's not implemented dude.

    tileGrid = grid;
    this->tileSet = std::move(tileSet);
    this->gridIndex = gridIndex;
    this->chunkSize = chunkSize;
    this->dimensions = dimensions;
    this->layer = layer;

    int dimSize = dimensions.first * dimensions.second;

    tiles.reserve(dimSize);
    tileWorldPositions.reserve(dimSize);
    tileHasCollider.reserve(dimSize);
    for (int y = 0; y < dimensions.second; y ++)
    {
        for (int x = 0; x < dimensions.first; x++)
        {
            tileWorldPositions.emplace_back(tileGrid->GridToWorldSpace({x + gridIndex.first,y + gridIndex.second}));
            tileHasCollider.emplace_back(false);
            tiles.push_back(-1);
        }
    }
    tilesToRender.reserve(dimSize);
    GeneratePartitions();
}

glm::vec2 TileMap::CullMap(AABB camAABB, unsigned int pixelsPerUnit)
{
    //BROAD PHASE

    AABB chunkAABB = {0,0,0,0};
    for (int i = 0; i < partitions.size(); i++)
    {
        chunkAABB.min = {partitions[i].x, partitions[i].y} * (float)pixelsPerUnit;
        chunkAABB.max = {partitions[i].z, partitions[i].w} * (float)pixelsPerUnit;

        if (AABB::Intersect(camAABB, chunkAABB))
        {
            chunksToRender.push_back(i);
        }
    }

    //NARROW PHASE

    tilesToRender.clear();

    float minTileY = std::numeric_limits<float>::infinity();
    float maxTileY = -std::numeric_limits<float>::infinity();

    //TODO ONLY RENDER VALID CHUNKS

    //Half tile
    glm::vec2 tileSize = tileGrid->GetTileSize() * 0.5f;

    AABB tileAABB = {0,0,0,0};

    for (int i = 0; i < chunksToRender.size(); i++)
    {
        int startX = (i % partitionDimensions.first) * (int)chunkSize, startY = (i / partitionDimensions.first) * (int)chunkSize;

        for (int x = startX; x < startX + chunkSize; x++)
        {
            if (x >= dimensions.first)
            {
                break;
            }

            for (int y = startY; y < startY + chunkSize; y++)
            {
                if (y >= dimensions.second)
                {
                    break;
                }

                int index = FindIndexOfTile(x, y);
                if (tiles[index] > -1)
                {
                    tileAABB.min = (tileWorldPositions[index] - tileSize) * (float)pixelsPerUnit;
                    tileAABB.max = (tileWorldPositions[index] + tileSize) * (float)pixelsPerUnit;
                    if (AABB::Intersect(camAABB, tileAABB))
                    {
                        tilesToRender.emplace_back(tileWorldPositions[index], tiles[index], 0);
                        if (tileWorldPositions[index].y < minTileY)
                        {
                            minTileY = tileWorldPositions[index].y;
                        }
                        if (tileWorldPositions[index].y > maxTileY)
                        {
                            maxTileY = tileWorldPositions[index].y;
                        }
                    }
                }
            }
        }
    }

    return {minTileY, maxTileY};
}

void TileMap::GenerateColliders()
{
    updateColliderFlag = true;
    tileColliders.clear();
    int xIndex = 0, yIndex = 0;
    std::pair<int,int> currentMin = {0,0};
    std::pair<int,int> currentMax = {0,0};
    std::vector<bool> colliderCheck(tileHasCollider);
    glm::vec2 tSize = tileGrid->GetTileSize();
    glm::vec2 tHalfWidth = tSize * 0.5f;

    for (; yIndex < dimensions.second; yIndex++)
    {
        for (; xIndex < dimensions.first; xIndex++)
        {
            int index = FindIndexOfTile(xIndex, yIndex);
            if (colliderCheck[index])
            {
                //Begin chunk.
                currentMin = {xIndex, yIndex};
                currentMax = {xIndex, yIndex};

                int xSize = xIndex + 1;
                int ySize = yIndex;
                //Find length of chunk
                while (xSize < dimensions.first && colliderCheck[FindIndexOfTile(xSize, ySize)])
                {
                    currentMax = {xSize, ySize};
                    xSize++;
                }

                //Find height of chunk
                ySize++;
                bool canExpand = true;
                while (ySize < dimensions.second && canExpand)
                {
                    for (int x = currentMin.first; x <= currentMax.first; x++)
                    {
                        if (!colliderCheck[FindIndexOfTile(x, ySize)])
                        {
                            canExpand = false;
                            break;
                        }
                    }
                    if (canExpand)
                    {
                        currentMax.second++;
                        ySize++;
                    }
                }

                //Add points
                tileColliders.emplace_back();
                //If only a single tile in the collider.
                if (currentMin.first == currentMax.first && currentMin.second == currentMax.second)
                {
                    glm::vec2 pos = tileWorldPositions[FindIndexOfTile(currentMin.first, currentMin.second)];
                    //Top left, clockwise
                    if (tileGrid->GetShape() == GridShape::GRID_RECTANGULAR)
                    {
                        tileColliders.back().vertices[0] = {pos + glm::vec2(0, tSize.y)};
                        tileColliders.back().vertices[1] = {pos + tSize};
                        tileColliders.back().vertices[2] = {pos + glm::vec2(tSize.x, 0)};
                        tileColliders.back().vertices[3] = {pos};
                    } else if (tileGrid->GetShape() == GridShape::GRID_ISOMETRIC)
                    {
                        //left clockwise
                        tileColliders.back().vertices[0] = {pos + glm::vec2(0, tHalfWidth.y)};
                        tileColliders.back().vertices[1] = {pos + glm::vec2(tHalfWidth.x, tSize.y)};
                        tileColliders.back().vertices[2] = {pos + glm::vec2(tSize.x, tHalfWidth.y)};
                        tileColliders.back().vertices[3] = {pos + glm::vec2(tHalfWidth.x, 0)};
                    }
                } else
                {
                    glm::vec2 minPos = tileWorldPositions[FindIndexOfTile(currentMin.first, currentMin.second)];
                    glm::vec2 maxPos = tileWorldPositions[FindIndexOfTile(currentMax.first, currentMax.second)];

                    if (tileGrid->GetShape() == GridShape::GRID_RECTANGULAR)
                    {
                        tileColliders.back().vertices[0] = {glm::vec2(minPos.x, maxPos.y) + glm::vec2(0, tSize.y)};
                        tileColliders.back().vertices[1] = {maxPos + tSize};
                        tileColliders.back().vertices[2] = {glm::vec2(maxPos.x, minPos.y) + glm::vec2(tSize.x, 0)};
                        tileColliders.back().vertices[3] = {minPos};
                    } else if (tileGrid->GetShape() == GridShape::GRID_ISOMETRIC)
                    {
                        glm::vec2 topLeft = tileWorldPositions[FindIndexOfTile(currentMin.first, currentMax.second)];
                        glm::vec2 bottomRight = tileWorldPositions[FindIndexOfTile(currentMax.first, currentMin.second)];
                        //Left clockwise
                        tileColliders.back().vertices[0] = {minPos + glm::vec2(0, tHalfWidth.y)};
                        tileColliders.back().vertices[1] = {topLeft + glm::vec2(tHalfWidth.x, tSize.y)};
                        tileColliders.back().vertices[2] = {maxPos + glm::vec2(tSize.x, tHalfWidth.y)};
                        tileColliders.back().vertices[3] = {bottomRight + glm::vec2(tHalfWidth.x, 0)};
                    }
                }
                tileColliders.back().InitAABB();
                tileColliders.back().collisionTag = collisionTag;
                tileColliders.back().material = &physMaterial;

                //Clear data from colliderCheck
                for (int i = currentMin.first; i <= currentMax.first; i++)
                {
                    for (int j = currentMin.second; j <= currentMax.second; j++)
                    {
                        colliderCheck[FindIndexOfTile(i, j)] = false;
                    }
                }
            }
        }
        xIndex = 0;
    }
}


void TileMap::GeneratePartitions()
{
    //Bounding box positions will account for partial chunks in the broad phase.
    unsigned int xPartitions = (dimensions.first / chunkSize);
    unsigned int yPartitions = (dimensions.second / chunkSize);

    unsigned int modX, modY;

    if (xPartitions == 0)
    {
        modX = dimensions.first;
    } else
    {
        modX = dimensions.first % xPartitions;
    }

    if (yPartitions == 0)
    {
        modY = dimensions.second;
    } else
    {
        modY = dimensions.second % yPartitions;
    }

    if (modX > 0)
    {
        xPartitions++;
    }
    if (modY > 0)
    {
        yPartitions++;
    }

    partitionDimensions = {xPartitions, yPartitions};

    partitions.clear();

    if (yPartitions == 1 && xPartitions == 1)
    {
        float minX = std::numeric_limits<float>::infinity(), minY = std::numeric_limits<float>::infinity();
        float maxX = -std::numeric_limits<float>::infinity(), maxY = -std::numeric_limits<float>::infinity();

        //Starts in bottom left, winds clockwise
        std::pair<int,int> gridPositions[4] = {{gridIndex.first, gridIndex.second},
            {gridIndex.first, gridIndex.second + dimensions.second - 1},
            {gridIndex.first + dimensions.first - 1, gridIndex.second + dimensions.second - 1},
            {gridIndex.first + dimensions.first - 1, gridIndex.second}};

        glm::vec2 edgePoints[4];
        edgePoints[0] = tileGrid->GridToWorldSpace(gridPositions[0]);
        edgePoints[1] = tileGrid->GridToWorldSpace(gridPositions[1]);
        edgePoints[2] = tileGrid->GridToWorldSpace(gridPositions[2]);
        edgePoints[3] = tileGrid->GridToWorldSpace(gridPositions[3]);

        for (auto & edgePoint : edgePoints)
        {
            if (edgePoint.x > maxX)
            {
                maxX = edgePoint.x;
            }
            if (edgePoint.x < minX)
            {
                minX = edgePoint.x;
            }
            if (edgePoint.y > maxY)
            {
                maxY = edgePoint.y;
            }
            if (edgePoint.y < minY)
            {
                minY = edgePoint.y;
            }
        }

        partitions.emplace_back(minX, minY, maxX, maxY);
        return;
    }

    for (int x = 0; x < xPartitions; x++)
    {
        for (int y = 0; y < yPartitions; y++)
        {
            float minX = std::numeric_limits<float>::infinity(), minY = std::numeric_limits<float>::infinity();
            float maxX = -std::numeric_limits<float>::infinity(), maxY = -std::numeric_limits<float>::infinity();

            //Starts in bottom left, winds clockwise
            std::pair<int,int> gridPositions[4] = {{x * chunkSize, y * chunkSize},
                {x * chunkSize, y * chunkSize + (chunkSize - 1)},
                {x * chunkSize + (chunkSize - 1), y * chunkSize + (chunkSize - 1)},
                {x * chunkSize + (chunkSize - 1), y * chunkSize}};

            glm::vec2 edgePoints[4];
            edgePoints[0] = tileGrid->GridToWorldSpace(gridPositions[0]);
            edgePoints[1] = tileGrid->GridToWorldSpace(gridPositions[1]);
            edgePoints[2] = tileGrid->GridToWorldSpace(gridPositions[2]);
            edgePoints[3] = tileGrid->GridToWorldSpace(gridPositions[3]);

            for (auto & edgePoint : edgePoints)
            {
                if (edgePoint.x > maxX)
                {
                    maxX = edgePoint.x;
                }
                if (edgePoint.x < minX)
                {
                    minX = edgePoint.x;
                }
                if (edgePoint.y > maxY)
                {
                    maxY = edgePoint.y;
                }
                if (edgePoint.y < minY)
                {
                    minY = edgePoint.y;
                }
            }

            partitions.emplace_back(minX, minY, maxX, maxY);
        }
    }
}

int TileMap::FindIndexOfTile(int x, int y) const
{
    return (y * dimensions.first) + x;
}






