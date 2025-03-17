//
// Created by Somed on 3/16/2025.
//

#pragma once

#include "UIRenderer.h"
#include "UIBehaviorManager.h"
#include "EngineStuffs/Graphics/RenderSettings.h"

class UISystem
{
public:

    static UISystem* getInstance();
    static UISystem* createInstance(RenderSettings render_settings);
    static void delInstance();

    void RenderUI()
    {
        if (uiRenderer != nullptr)
        {
            unsigned int lastUIObj = uiPool->GetActiveLine();
            ActiveTracker<UIObject*> uis = uiPool->getPool();
            uiRenderer->RenderUI(uis, lastUIObj);
            uiPool->CompactPool(uis.GetNumInactive());
        }
    }

    void UpdateUILayer(UIObject* ui_object)
    {
        if (uiRenderer != nullptr)
        {
            uiRenderer->OrderUI(ui_object);
        }
    }

    LilObj<UIObject> addUIObject(const std::string &name, const std::string &image, unsigned int frame, unsigned int layer,
        glm::vec2 screenPos, glm::vec2 renderSize);
    //void setUIObjectLayer(UIObject* uiObj);
    ActiveTracker<UIObject*> getUIObjectsRaw() const {return uiPool->getPool(); };
    unsigned int getUIActive() {return uiPool->GetActiveLine(); };
    void compactUIObjects(int active) {uiPool->CompactPool(active);}
    void clearUIObjects() {uiPool->ClearPool();};

private:

    UIRenderer* uiRenderer;
    UIBehaviorManager* uiBehaviorManager;

    static inline UISystem* instance;

    ComponentPool<UIObject>* uiPool;

    UISystem(RenderSettings render_settings);
    ~UISystem()
    {
        if (uiRenderer != nullptr)
        {
            delete uiRenderer;
            uiRenderer = nullptr;
        }
        if (uiBehaviorManager != nullptr)
        {
            delete uiBehaviorManager;
            uiBehaviorManager = nullptr;
        }
        delete uiPool;
        uiPool = nullptr;
    }
};
