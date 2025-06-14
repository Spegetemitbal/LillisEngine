//
// Created by Somed on 6/12/2025.
//

#ifndef SPATIALHASHER_H
#define SPATIALHASHER_H

#include <unordered_set>

#include "Physics/AABB.h"

struct GridHash
{
    GridHash() = default;
    GridHash(int width, int height)
    {
        x = width;
        y = height;
    }
    int x = 0, y = 0;

    bool operator==(const GridHash &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator<(const GridHash &rhs) const
    {
        return x < rhs.x && y <= rhs.y || y < rhs.y && x <= rhs.x;
    }
};

namespace std
{
    template<>
    struct hash<GridHash>
    {
        size_t operator()(const GridHash& p) const
        {
            // Combine hashes of x and y using the bitwise XOR
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };

}

class SpatialHasher {
public:
    SpatialHasher() = default;
    ~SpatialHasher() = default;

    void SetTileSize(float sz);
    std::unordered_set<GridHash>& GetGridSpaces() {return keys;}
    std::pair<std::multimap<GridHash, int>::iterator,
    std::multimap<GridHash, int>::iterator> GetObjectsInGrid(GridHash g);
    std::pair<std::multimap<GridHash, std::pair<int,int>>::iterator,
    //Could very well be 0
    std::multimap<GridHash, std::pair<int,int>>::iterator> GetTilesInGrid(GridHash g);
    void InsertObject(AABB aabb, int iter);
    void InsertTile(AABB aabb, int xIter, int yIter);
    //For later usage.
    void DelTile(AABB aabb, int xIter, int yIter);
    void Clear();
    void ClearTiles();

private:
    float tileSize = 10.0f;
    float invTileSize = 0.1f;
    //Multimap of pairs here (x,y grid pairs), make it work.
    std::unordered_set<GridHash> keys;
    //int is location in the array.
    std::multimap<GridHash, int> spatialHash;
    std::multimap<GridHash, std::pair<int,int>> tileHash;
};



#endif //SPATIALHASHER_H
