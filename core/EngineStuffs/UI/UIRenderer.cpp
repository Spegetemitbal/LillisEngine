//
// Created by Somed on 3/15/2025.
//

#include "UIRenderer.h"

#include "EngineStuffs/Graphics/SpriteRenderer.h"
#include "EngineStuffs/Graphics/Texture.h"
#include "glad/gl.h"
#include "Utils/ResourceManager.h"

UIRenderer::UIRenderer(unsigned int uiWidth, unsigned int uiHeight, ComponentPool<UIObject>* uiObjectPool)
{
    screenWidth = uiWidth;
    screenHeight = uiHeight;
    uiPool = uiObjectPool;

    //Generate frame buffer.
    glGenFramebuffers(1, &UIFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);

    //8 bit RGBA color buffer
    glGenTextures(1, &UIColorBuffer);
    glBindTexture(GL_TEXTURE_2D, UIColorBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, screenWidth, screenHeight);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, UIColorBuffer, 0);

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer incomplete: %d", fboStatus);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

UIRenderer::~UIRenderer()
{
    glDeleteFramebuffers(1, &UIFBO);
}

void UIRenderer::OrderUI(UIObject* uiObj)
{
    unsigned int layer = uiObj->getLayer();
    unsigned int EOP = uiPool->activeLine - 1;
    unsigned int uiIndex = uiPool->FindObjectIndex(uiPool->GetObjByID<UIObject>(uiObj->GetID()));

    if (uiIndex == 0)
    {
        if (EOP == 0)
        {
            return;
        }
        while (uiIndex < EOP)
        {
            if (layer > uiPool->poolDir[uiIndex + 1]->getLayer())
            {
                uiPool->SwapObjects(uiPool->poolDir[uiIndex], uiPool->poolDir[uiIndex + 1]);
                uiIndex++;
            } else
            {
                break;
            }
        }
    } else if (uiIndex == EOP)
    {
        while (uiIndex > 0)
        {
            if (layer < uiPool->poolDir[uiIndex - 1]->getLayer())
            {
                uiPool->SwapObjects(uiPool->poolDir[uiIndex], uiPool->poolDir[uiIndex - 1]);
                uiIndex--;
            } else
            {
                break;
            }
        }
    } else
    {
        bool goingLeft = true;
        if (layer > uiPool->poolDir[uiIndex + 1]->getLayer())
        {
            goingLeft = false;
        }

        if (goingLeft)
        {
            while (uiIndex > 0)
            {
                if (layer < uiPool->poolDir[uiIndex - 1]->getLayer())
                {
                    uiPool->SwapObjects(uiPool->poolDir[uiIndex], uiPool->poolDir[uiIndex - 1]);
                    uiIndex--;
                } else
                {
                    break;
                }
            }
        } else
        {
            while (uiIndex < EOP)
            {
                if (layer > uiPool->poolDir[uiIndex + 1]->getLayer())
                {
                    uiPool->SwapObjects(uiPool->poolDir[uiIndex], uiPool->poolDir[uiIndex + 1]);
                    uiIndex++;
                } else
                {
                    break;
                }
            }
        }
    }


}

void UIRenderer::RenderUI(ActiveTracker<UIObject *> &uiObjs, unsigned int lastUIObj)
{
    for (int i = 0; i < lastUIObj; i++)
    {
        UIObject* spr = uiObjs[i];

        glm::vec2 renderPos = {spr->screenPos.x * screenWidth, spr->screenPos.y * screenHeight};
        glm::vec2 renderSize = {spr->renderSize.x * screenWidth, spr->renderSize.y * screenHeight};

        if (spr->image.empty())
        {
            throw;
        }
        Texture2D tex = ResourceManager::GetTexture(spr->image);
        SpriteRenderer::DrawUI(tex, renderPos, spr->frame,
            renderSize, 0);
    }
}

