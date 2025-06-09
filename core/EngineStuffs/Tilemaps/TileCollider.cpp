//
// Created by Somed on 6/8/2025.
//

#include "TileCollider.h"

void TileCollider::InitAABB()
{
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < 4; i++)
    {
        //For polygons, this is the same thing dude.
        glm::vec2 v = vertices[i];
        if (v.x < minX) {minX = v.x;}
        if (v.x > maxX) {maxX = v.x;}
        if (v.y < minY) {minY = v.y;}
        if (v.y > maxY) {maxY = v.y;}
    }

    aabb = {minX, minY, maxX, maxY};

    center = (vertices[0] + vertices[1] + vertices[2] + vertices[3]) * 0.25f;
}
