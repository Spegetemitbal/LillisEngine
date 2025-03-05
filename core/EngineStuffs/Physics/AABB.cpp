//
// Created by Somed on 2/19/2025.
//

#include "AABB.h"

void AABB::UpdateBox(glm::vec2 vert[4])
{
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < 4; i++)
    {
        //For polygons, this is the same thing dude.
        glm::vec2 v = vert[i];
        if (v.x < minX) {minX = v.x;}
        if (v.x > maxX) {maxX = v.x;}
        if (v.y < minY) {minY = v.y;}
        if (v.y > maxY) {maxY = v.y;}
    }

    this->min = {minX, minY};
    this->max = {maxX, maxY};
}

bool AABB::Intersect(AABB a, AABB b)
{
    if (a.max.x <= b.min.x || b.max.x <= a.min.x
        || a.max.y <= b.min.y || b.max.y <= a.min.y)
    {
        return false;
    }

    return true;
}

