//
// Created by Somed on 6/3/2025.
//

#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H
#include <glm/vec2.hpp>


class ParticleEffect {
    std::string image;
    unsigned int frame = 0;
    glm::vec2 startSize = glm::vec2(1);
};



#endif //PARTICLEEFFECT_H
