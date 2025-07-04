//
// Created by Somed on 6/26/2025.
//

#include "BackgroundManager.h"
#include <glad/gl.h>

#include "Parallax.h"
#include "Texture.h"
#include "Utils/ResourceManager.h"

BackgroundManager::BackgroundManager()
{

}

BackgroundManager::~BackgroundManager()
{

}


std::vector<BackgroundImage> BackgroundManager::GetBackgrounds(LILLIS::Camera& camera, bool doParallax)
{
    std::vector<BackgroundImage> returnable = std::vector<BackgroundImage>();

    glm::vec4 cameraAABB = camera.getAABB(1);

    glm::vec2 xRange = {cameraAABB.x, cameraAABB.z};
    glm::vec2 yRange = {cameraAABB.y, cameraAABB.w};
    glm::vec2 topLeft = {xRange.x, yRange.y};


    for (int i = 0; i < backgroundDatas.size(); i++)
    {
        if (backgroundDatas[i].fixed)
        {
            returnable.emplace_back(&backgroundDatas[i], topLeft);
            continue;
        }

        glm::vec2 Disp = backgroundDatas[i].basePosition;
        if (doParallax)
        {
            Disp += Parallax::doParallaxOffset(
                backgroundDatas[i].layer, backgroundDatas[i].basePosition, camera.position);
        }

        glm::vec2 invSize = {1 / backgroundDatas[i].imageSize.x, 1 / backgroundDatas[i].imageSize.y};

        int horMin = std::floor((xRange.x - Disp.x) * invSize.x);
        int horMax = std::ceil((xRange.y - Disp.x) * invSize.x);
        int vertMin = std::floor((yRange.x - Disp.y) * invSize.y);
        int vertMax = std::ceil((yRange.y - Disp.y) * invSize.y);

        for (int x = horMin; x < horMax; x++)
        {
            for (int y = vertMax; y > vertMin; y--)
            {
                glm::vec2 displacement = {((float)x * backgroundDatas[i].imageSize.x) + Disp.x,
                    ((float)y * backgroundDatas[i].imageSize.y) + Disp.y};
                if (!backgroundDatas[i].hasHorizontal)
                {
                    if (Disp.x != displacement.x)
                    {
                        continue;
                    }
                }
                if (!backgroundDatas[i].hasVertical)
                {
                    if (Disp.y != displacement.y)
                    {
                        continue;
                    }
                }
                returnable.emplace_back(&backgroundDatas[i], displacement);
            }
        }
    }

    return returnable;
}

void BackgroundManager::TickBackgrounds(float dt)
{
    for (auto it : backgroundDatas)
    {
        if (!it.imageFrames.empty())
        {
            it.currentTime += dt;
            if (it.currentTime >= it.animSpeed)
            {
                it.currentTime = 0.0f;
                it.currentFrame++;
                if (it.currentFrame >= it.imageFrames.size())
                {
                    it.currentFrame = 0;
                }
            }
        }
    }
}


bool BackgroundManager::AddBackground(const BackgroundData& background)
{
    int maxBackgrounds;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxBackgrounds);

    if (backgroundDatas.size() + 1 > maxBackgrounds)
    {
        std::cout << "Maximum backgrounds (" << maxBackgrounds << ") reached, cannot add: " << background.image << '\n';
        return false;
    }

    for (auto & backgroundImage : backgroundDatas)
    {
        if (backgroundImage.layer == background.layer)
        {
            std::cout << "Only one background per layer." << '\n';
            return false;
        }
    }
    backgroundDatas.push_back(background);
    std::sort(backgroundDatas.begin(), backgroundDatas.end());
    return true;
}

void BackgroundManager::RemoveBackgroundLayer(unsigned int layer)
{
    for (auto it = backgroundDatas.begin(); it != backgroundDatas.end(); it++)
    {
        if (it->layer == layer)
        {
            backgroundDatas.erase(it);
            return;
        }
    }
}
