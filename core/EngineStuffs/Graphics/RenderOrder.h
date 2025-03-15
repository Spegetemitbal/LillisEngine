//
// Created by Somed on 12/30/2024.
//

#ifndef RENDERORDER_H
#define RENDERORDER_H

#include <unordered_set>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "../MemoryManagement/ComponentPool.h"

class Sprite;

class RenderOrder {
public:
    RenderOrder() = delete;
    ~RenderOrder() = default;
    explicit RenderOrder(ComponentPool<Sprite>* sprPool) {spritePool = sprPool;}

    static void CalculateOrder(std::vector<Sprite*>& spritesOnScreen, const float& up, const float& down);

    //Currently only supporting upwards axis
    static void SetRenderAxis(bool axUp)
    {
        if (axUp && highestLayer < maxLayers)
        {
            axis = {0,1};
        } else
        {
            axis = {};
        }
    }

    void MoveSprite(LilObj<Sprite> spr);
private:
    int compareAxis(Sprite* const &spr1, Sprite* const &spr2);
    ComponentPool<Sprite>* spritePool;
    std::vector<unsigned int> layerIndices;

    static inline glm::vec2 axis;
    static inline unsigned int highestLayer = 0;

    //A quick safety feature.
    //TODO: add engine settings to change the precision.
    const static inline unsigned int maxLayers = 20;
    const static inline float NEAR_PLANE = 10.0f;
    const static inline float FAR_PLANE = 20.0f;
    const static inline float AXIS_BUFFER = 0.98f;
};



#endif //RENDERORDER_H
