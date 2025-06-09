//
// Created by Somed on 6/8/2025.
//

#ifndef TILECOLLIDER_H
#define TILECOLLIDER_H
#include <glm/vec2.hpp>

#include "EngineStuffs/Physics/AABB.h"
#include "EngineStuffs/Physics/PhysicsMaterial.h"

class TileCollider {
public:
    glm::vec2 vertices[4] = {};
    glm::vec2 center = {};
    AABB aabb = AABB(0,0,0,0);
    int collisionTag = 0;
    PhysicsMaterial material = PhysicsMaterial();
    void InitAABB();
private:
};



#endif //TILECOLLIDER_H
