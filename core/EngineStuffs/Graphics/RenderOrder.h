//
// Created by Somed on 12/30/2024.
//

#ifndef RENDERORDER_H
#define RENDERORDER_H

#include "../MemoryManagement/ComponentPool.h"

class Sprite;

class RenderOrder {
public:
    RenderOrder() = delete;
    ~RenderOrder() = default;
    explicit RenderOrder(ComponentPool<Sprite>* sprPool) {spritePool = sprPool;}

    void MoveSprite(LilObj<Sprite> spr);
private:
    ComponentPool<Sprite>* spritePool;
};



#endif //RENDERORDER_H
