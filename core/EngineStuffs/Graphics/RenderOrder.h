//
// Created by Somed on 12/30/2024.
//

#ifndef RENDERORDER_H
#define RENDERORDER_H

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "../MemoryManagement/ComponentPool.h"

class Sprite;

class RenderOrder {
public:
    RenderOrder() = delete;
    ~RenderOrder() = default;
    explicit RenderOrder(ComponentPool<Sprite>* sprPool) {spritePool = sprPool;}

    void OrderByAxis();

    void SetRenderAxis(glm::vec3 ax)
    {
        if (ax == glm::vec3(0))
        {
            axis = ax;
            return;
        }
        axis = glm::normalize(ax);
    }

    void MoveSprite(LilObj<Sprite> spr);
private:
    bool compareAxis(Sprite* const &spr1, Sprite* const &spr2);
    ComponentPool<Sprite>* spritePool;
    std::vector<int> layerIndices;
    glm::vec3 axis;
};



#endif //RENDERORDER_H
