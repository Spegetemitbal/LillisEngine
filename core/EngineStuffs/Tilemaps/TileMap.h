//
// Created by Somed on 3/17/2025.
//

#ifndef TILEMAP_H
#define TILEMAP_H
#include <glm/vec4.hpp>
#include <utility>

#include "TileGrid.h"
#include "EngineStuffs/Physics/AABB.h"

struct TileData
{
    std::string image;
    float height;
    unsigned int frame;
};

struct TileSet
{
    std::map<char, int> inputConversion;
    std::vector<TileData> tileSet;
};

struct TileLoc
{
    glm::vec2 worldPos;
    int tile;
    float zVal;
};

class TileMap {
public:

    TileMap();
    TileMap(TileGrid* grid, TileSet tileSet, std::pair<int, int> gridIndex, std::pair<int, int> dimensions, unsigned int chunkSize = 20);
    //Has no data ownership.
    ~TileMap() = default;

    void setTile(std::pair<int, int> index, char tile);

    glm::vec2 getTileSize() const
    {
        if (tileGrid != nullptr)
        {
            return tileGrid->GetTileSize();
        } else
        {
            return {};
        }
    }

    void setTileSet(TileSet tileSet)
    {
        this->tileSet = std::move(tileSet);
    }

    void setTileGrid(TileGrid* tileGrid)
    {
        this->tileGrid = tileGrid;
    }

    std::string getImageFromIndex(int index)
    {
        if (index < 0 || index >= this->tileSet.tileSet.size())
        {
            return {""};
        }
        return this->tileSet.tileSet[index].image;
    }

    unsigned int getFrameFromIndex(int index)
    {
        if (index < 0 || index >= this->tileSet.tileSet.size())
        {
            return 0;
        }
        return this->tileSet.tileSet[index].frame;
    }

    //x value is low, y value is high.
    glm::vec2 CullMap(AABB camAABB, unsigned int pixelsPerUnit);

    std::vector<TileLoc> tilesToRender;
    unsigned int layer = 0;
    bool active = true;
private:

    void GeneratePartitions();
    std::pair<int, int> partitionDimensions;
    std::vector<glm::vec4> partitions;

    std::vector<int> chunksToRender;

    int FindIndexOfTile(int x, int y) const;

    //Assume each tilemap is neatly packed.

    std::pair<int, int> gridIndex = { 0, 0 }, dimensions = { 5, 5 };
    unsigned int chunkSize = 20;

    //-1 is empty.
    std::vector<int> tiles;
    std::vector<glm::vec2> tileWorldPositions;
    TileSet tileSet;
    TileGrid* tileGrid;
};



#endif //TILEMAP_H
