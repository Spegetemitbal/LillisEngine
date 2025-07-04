//
// Created by Somed on 7/2/2025.
//

#ifndef PARALLAX_H
#define PARALLAX_H
#include <unordered_set>
#include <glm/vec2.hpp>


class Parallax {
public:
    static void setCenterLayer(int layer, float centerDistance);
    static int getCenterLayer() {return centerLayer;}
    static float getCenterDistance() {return centerDist;}

    static void clearParallax()
    {
        parallaxLayers.clear();
        centerLayer = 0;
    }

    static glm::vec2 doParallaxOffset(int layer, glm::vec2 itemCenter, glm::vec2 cameraCenter);

    static bool isLayerParallax(int layer) {return parallaxLayers.contains(layer);}
    static void AddLayerToParallax(int layer);
    static void RemoveLayerFromParallax(int layer);

    static float getDistBetweenLayers() {return distBetweenLayers;}
    static void setDistBetweenLayers(float distanceBetweenLayers)
    {
        if (distanceBetweenLayers <= 1.0f)
        {
            distanceBetweenLayers = 1.0f;
        }
        distBetweenLayers = distanceBetweenLayers;
    };

    static inline bool doHorizontal = true, doVertical = true;
private:

    static inline std::unordered_set<int> parallaxLayers = std::unordered_set<int>();
    static inline float centerDist = 10;
    static inline float distBetweenLayers = 5;
    static inline int centerLayer = 0;
};



#endif //PARALLAX_H
