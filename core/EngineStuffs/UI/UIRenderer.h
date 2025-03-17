//
// Created by Somed on 3/15/2025.
//

#ifndef UIRENDERER_H
#define UIRENDERER_H
#include <glm/vec2.hpp>

#include "UIObject.h"
#include "EngineStuffs/MemoryManagement/ComponentPool.h"


class UIRenderer {
public:
    UIRenderer(unsigned int uiWidth, unsigned int uiHeight, ComponentPool<UIObject>* uiObjectPool);
    ~UIRenderer();

    void RenderUI(ActiveTracker<UIObject*>& uiObjs, unsigned int lastUIObj);
    void OrderUI(UIObject* uiObj);

private:
    ComponentPool<UIObject>* uiPool;
    std::vector<unsigned int> layerIndices;

    unsigned int screenWidth, screenHeight;
    unsigned int UIFBO{}, UIColorBuffer{};
};



#endif //UIRENDERER_H
