//
// Created by Somed on 2/19/2025.
//

#ifndef AABB_H
#define AABB_H
#include <glm/vec2.hpp>


struct AABB {
    glm::vec2 min = {}, max = {};

    AABB(glm::vec2 min, glm::vec2 max)
    {
        this->min = min;
        this->max = max;
    }

    AABB(float minX, float minY, float maxX, float maxY)
    {
        this->min = glm::vec2(minX, minY);
        this->max = glm::vec2(maxX, maxY);
    }

    AABB Scale(float scalar) const
    {
        return {min * scalar, max * scalar};
    }

    void UpdateBox(glm::vec2 vert[4]);

    static bool Intersect(AABB a, AABB b);
};



#endif //AABB_H
