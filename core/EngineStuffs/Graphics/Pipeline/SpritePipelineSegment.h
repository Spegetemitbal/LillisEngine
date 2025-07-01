//
// Created by Somed on 6/16/2025.
//

#ifndef SPRITEPIPELINESEGMENT_H
#define SPRITEPIPELINESEGMENT_H
#include "PipelineSegment.h"
#include "EngineStuffs/Graphics/Sprite.h"
#include "EngineStuffs/Graphics/Texture.h"
#include "EngineStuffs/Tilemaps/TileMap.h"

struct BackgroundImage;

class SpritePipelineSegment : public PipelineSegment
{
public:
    SpritePipelineSegment(RenderSettings render_settings, LILLIS::Shader shader);
    SpritePipelineSegment() = delete;

    void InitSegment() override;
    void PreRender() override;
    virtual std::vector<ColorBufferWrapper> DoStep(std::vector<Sprite*>& sprites, unsigned int lastSprite, std::vector<TileMap>& tile_maps, glm::mat4 camera);
protected:
    virtual void RenderSprite(const Texture2D& texture, glm::vec2 position, float renderVal, int frame, glm::mat4 camera,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));
};



#endif //SPRITEPIPELINESEGMENT_H
