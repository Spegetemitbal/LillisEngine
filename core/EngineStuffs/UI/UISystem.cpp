//
// Created by Somed on 3/16/2025.
//

#include "UISystem.h"

#include "EngineStuffs/Graphics/GraphicsSystem.h"

UISystem::UISystem(RenderSettings render_settings)
{
    uiPool = DBG_NEW ComponentPool<UIObject>(50);
    uiRenderer = DBG_NEW UIRenderer(render_settings.windowWidth, render_settings.windowHeight, uiPool);
    uiBehaviorManager = DBG_NEW UIBehaviorManager();
}

LilObj<UIObject> UISystem::addUIObject(const std::string &name, const std::string &image, unsigned int frame, unsigned int layer,
        glm::vec2 screenPos, glm::vec2 renderSize)
{
    UIObject* ui = uiPool->AddComponent();
    ui->name = name;
    ui->image = image;
    ui->frame = frame;
    ui->renderSize = renderSize;
    ui->screenPos = screenPos;
    ui->SetLayer(layer);

    return {uiPool, ui->GetID()};
}



UISystem* UISystem::getInstance()
{
    if (instance == nullptr)
    {
        return createInstance();
    }
    return instance;
}

//Singleton init
UISystem* UISystem::createInstance(RenderSettings render_settings)
{
    if (instance == nullptr)
    {
        if (!GraphicsSystem::getInstance()->GetIsInitted())
        {
            std::cerr << "Cannot create UISystem instance without initialized GraphicsSystem" << std::endl;
            return nullptr;
        }
        instance = DBG_NEW UISystem(render_settings);
    }
    return instance;
}

//Singleton delete
void UISystem::delInstance()
{
    delete instance;
    instance = nullptr;
}
