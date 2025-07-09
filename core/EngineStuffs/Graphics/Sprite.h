//
// Created by Somed on 12/30/2024.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <glm/vec2.hpp>

#include "../Component.h"
#include "../Transform.h"
#include "../Physics/AABB.h"

class Sprite : public Component
{
public:

    Sprite();
    ~Sprite() = default;

    explicit Sprite(const std::string& spr);

    std::string getImageName() {return image;}
    bool setImage(const std::string& img);
    bool getIsOpaque() const {return isOpaque;}

    glm::vec2 Offset() const {return offset;}
    glm::vec2 RenderSize() const {return renderSize;}

    void setOffset(const glm::vec2& offset)
    {
        this->offset = offset;
        InitVertices();
    }

    void setRenderSize(const glm::vec2& renderSize)
    {
        this->renderSize = renderSize;
        InitVertices();
    }

    unsigned int frame = 0;
    glm::vec2 getRenderLocation();
    glm::vec2 getRenderCenter();
    float getRenderRotation();
    glm::vec2 getRenderScale();
    unsigned int getLayer() const {return layer;}
    void SetLayer(unsigned int lyr);

    float getRenderValue() const {return renderValue;}

    LilObj<Transform> transform;
    AABB getAABB();

private:
    void LazySetTransform();

    void InitVertices();
    static glm::vec2 transformVertex(glm::vec2 v, glm::vec2 tr, float r);

    glm::vec2 verts[4]{};

    std::string image;
    bool isOpaque = true;

    AABB aabb;
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);
    glm::vec2 renderSize = glm::vec2(1.0f, 1.0f);

    friend class RenderOrder;

    float renderValue = 0.0f;

    unsigned int layer = 0;
};



#endif //SPRITE_H
