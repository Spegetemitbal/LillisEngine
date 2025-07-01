//
// Created by Somed on 6/26/2025.
//

#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct BackgroundData
{
    std::string image;
    float animSpeed = 1.0f;
    std::vector<int> imageFrames = std::vector<int>();

    int currentFrame = 0;
    float currentTime = 0.0f;

    unsigned int layer = 0;

    glm::vec2 basePosition = glm::vec2(0.0f);
    glm::vec2 imageSize = glm::vec2(1.0f);
    bool fixed = false, hasAnim = false, hasVertical = true, hasHorizontal = true;

    bool operator<(const BackgroundData& other) const
    {
        return layer < other.layer;
    }

};

struct BackgroundImage
{
    BackgroundData* data;
    glm::vec2 currentPosition = glm::vec2(0.0f);
};

class BackgroundManager {
public:
    BackgroundManager();
    ~BackgroundManager();
    std::vector<BackgroundImage> GetBackgrounds(glm::vec4 cameraAABB);
    void TickBackgrounds(float dt);
    void AddBackground(const BackgroundData& background);
    void RemoveBackgroundLayer(unsigned int layer);
    void SetBackgroundSlots(int backgroundSlots);
    int GetBackgroundSlots() const {return backgroundSlots;}
    void ClearBackgrounds() {backgroundDatas.clear();}

private:
    unsigned int SpriteLayerAbove = 0;
    int backgroundSlots = 0;
    std::vector<BackgroundData> backgroundDatas = std::vector<BackgroundData>();
};



#endif //BACKGROUNDMANAGER_H
