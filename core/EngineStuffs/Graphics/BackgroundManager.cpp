//
// Created by Somed on 6/26/2025.
//

#include "BackgroundManager.h"
#include <glad/gl.h>

#include "Texture.h"
#include "Utils/ResourceManager.h"

BackgroundManager::BackgroundManager()
{

}

BackgroundManager::~BackgroundManager()
{

}

void BackgroundManager::SetBackgroundSlots(int backgroundSlots)
{
    if (backgroundSlots < 1)
    {
        std::cout << "Invalid number of slots!" << '\n';
        return;
    }

    int maxBackgrounds;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxBackgrounds);

    if (backgroundSlots > maxBackgrounds)
    {
        std::cout << "Too many backgrounds!" << '\n';
        return;
    }

    this->backgroundSlots = backgroundSlots;
}


std::vector<BackgroundImage> BackgroundManager::GetBackgrounds(glm::vec4 cameraAABB)
{
    std::vector<BackgroundImage> returnable = std::vector<BackgroundImage>();

    glm::vec2 xRange = {cameraAABB.x, cameraAABB.z};
    glm::vec2 yRange = {cameraAABB.y, cameraAABB.w};

    for (int i = 0; i < backgroundDatas.size(); i++)
    {
        //No "Fixed" backgrounds, those can be done with a simple sprite following the camera.

        glm::vec2 invSize = {1 / backgroundDatas[i].imageSize.x, 1 / backgroundDatas[i].imageSize.y};

        int horMin = std::floor((xRange.x - backgroundDatas[i].basePosition.x) * invSize.x);
        int horMax = std::ceil((xRange.y - backgroundDatas[i].basePosition.x) * invSize.x);
        int vertMin = std::floor((yRange.x - backgroundDatas[i].basePosition.y) * invSize.y);
        int vertMax = std::ceil((yRange.y - backgroundDatas[i].basePosition.y) * invSize.y);

        for (int x = horMin; x < horMax; x++)
        {
            for (int y = vertMax; y > vertMin; y--)
            {
                glm::vec2 displacement = {((float)x * backgroundDatas[i].imageSize.x) + backgroundDatas[i].basePosition.x,
                    ((float)y * backgroundDatas[i].imageSize.y) + backgroundDatas[i].basePosition.y};
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


void BackgroundManager::AddBackground(const BackgroundData& background)
{
    if (backgroundDatas.size() >= backgroundSlots)
    {
        std::cout << "Too many backgrounds!" << '\n';
    }

    for (auto & backgroundImage : backgroundDatas)
    {
        if (backgroundImage.layer == background.layer)
        {
            std::cout << "Only one background per layer." << '\n';
            return;
        }
    }
    backgroundDatas.push_back(background);
    std::sort(backgroundDatas.begin(), backgroundDatas.end());
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
