//
// Created by Somed on 3/15/2025.
//

#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <glm/vec2.hpp>
#include "EngineStuffs/Component.h"

struct ButtonData
{
    bool isButton = false;
};

struct SliderData
{
    bool isSlider = false;
};

class UIObject : public Component
{
public:
    std::string image, name;
    unsigned int frame = 0;

    void SetLayer(unsigned int lyr);

    glm::vec2 renderSize = glm::vec2(1.0f, 1.0f);
    glm::vec2 screenPos = glm::vec2(0.0f, 0.0f);

    //TODO Make UIObjects archetypes with inheritance, build memory management specifically for them.
    SliderData sliderData = SliderData();
    ButtonData buttonData = ButtonData();

    unsigned int getLayer() const { return layer; }

private:
    unsigned int layer = 0;
};



#endif //UIOBJECT_H
