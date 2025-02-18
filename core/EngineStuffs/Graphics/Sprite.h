//
// Created by Somed on 12/30/2024.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>

#include "../Component.h"
#include "../Transform.h"

class Sprite : public Component
{
public:

    Sprite() = default;
    ~Sprite() = default;

    explicit Sprite(const std::string& spr);

    std::string image;
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);
    glm::vec2 renderSize = glm::vec2(0.0f, 0.0f);
    unsigned int frame = 0;
    glm::vec2 getRenderLocation();
    glm::vec2 getRenderCenter();
    float getRenderRotation();
    glm::vec2 getRenderScale();
    unsigned int getLayer() const {return layer;}
    void SetLayer(unsigned int lyr);
private:
    void LazySetTransform();

    LilObj<Transform> transform;
    friend class RenderOrder;
    unsigned int layer = 0;
};



#endif //SPRITE_H
