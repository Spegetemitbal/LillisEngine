//
// Created by Somed on 7/2/2025.
//

#include "Parallax.h"

void Parallax::setCenterLayer(int layer, float centerDistance)
{
    if (parallaxLayers.contains(layer))
    {
        centerLayer = layer;
        if (centerDistance <= 5.0f)
        {
            centerDistance = 5.0f;
        }
        centerDist = centerDistance;
        return;
    }
    std::cout << "Layer: " << layer << " is not in Parallax." << std::endl;
}

void Parallax::AddLayerToParallax(int layer)
{
    if (!parallaxLayers.contains(layer))
    {
        parallaxLayers.insert(layer);
    }
}

void Parallax::RemoveLayerFromParallax(int layer)
{
    if (centerLayer == layer)
    {
        std::cout << "Cannot remove center layer, try disabling parallax instead." << '\n';
        return;
    }
    if (parallaxLayers.contains(layer))
    {
        parallaxLayers.erase(layer);
    }
}

glm::vec2 Parallax::doParallaxOffset(int layer, glm::vec2 itemCenter, glm::vec2 cameraCenter)
{
    if (!parallaxLayers.contains(layer))
    {
        return {};
    }

    glm::vec2 distFromCam = itemCenter - cameraCenter;
    float dist2 = ((float)(centerLayer - layer) * distBetweenLayers);
    if (dist2 == 0)
    {
        //This should never happen.
        dist2 = 1;
    }
    return distFromCam * (centerDist / dist2);
}



