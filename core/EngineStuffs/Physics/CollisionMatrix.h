//
// Created by Somed on 7/27/2025.
//

#ifndef COLLISIONMATRIX_H
#define COLLISIONMATRIX_H
#include <unordered_set>

#include "../SpatialHasher.h"

class CollisionMatrix {
public:
    void AddCollisionExclusion(int layer1, int layer2);
    bool GetCollisionExclusion(int layer1, int layer2);
    void RemoveCollisionExclusion(int layer1, int layer2);
private:
    std::unordered_set<GridHash> collisionMatrix;
};



#endif //COLLISIONMATRIX_H
