//
// Created by Somed on 12/30/2024.
//

#ifndef RENDERORDER_H
#define RENDERORDER_H

#include <unordered_set>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "../MemoryManagement/ComponentPool.h"

class Sprite;

class RenderOrder {
public:
    RenderOrder() = delete;
    ~RenderOrder() = default;
    explicit RenderOrder(ComponentPool<Sprite>* sprPool) {spritePool = sprPool;}

    void OrderByAxis(const std::unordered_set<unsigned int>& toUpdate);
    void OrderAll();

    void SetRenderAxis(glm::vec3 ax)
    {
        if (ax == glm::vec3(0))
        {
            axis = ax;
            return;
        }
        axis = glm::normalize(ax);
    }

    void Clear()
    {
        sorted = false;
    }

    void MoveSprite(LilObj<Sprite> spr);
private:
    int compareAxis(Sprite* const &spr1, Sprite* const &spr2);
    ComponentPool<Sprite>* spritePool;
    std::vector<unsigned int> layerIndices;
    glm::vec3 axis;

    //Determines if the level has been loaded or not. To stop OrderByAxis from causing errors.
    bool sorted = false;

    //include low and high
    int Partition(int low, int high);
    void RenderSort(int low, int high);
    void SingleSort(int low, int high, std::unordered_set<unsigned int> toUpdate);
};



#endif //RENDERORDER_H
