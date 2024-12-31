//
// Created by Somed on 12/30/2024.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>

#include "../Component.h"

class Sprite : public Component
{
public:

    Sprite() = default;
    ~Sprite() = default;

    explicit Sprite(const std::string& spr);

    std::string image;
    glm::vec2 offset = glm::vec2(0, 0);
    glm::vec2 getRenderLocation();
    unsigned int getRenderRotation();
    glm::vec2 getRenderScale();
    unsigned int layer = 0;
};



#endif //SPRITE_H
