#pragma once

#include "Texture.h"
#include "Shader.h"
#include "ProcGenData.h"

class SpriteRenderer
{
public:

    static void setDefaultShader(LILLIS::Shader shader)
    {
        spriteShader = shader;
    }

    static void setDefaultUIShader(LILLIS::Shader shader)
    {
        uiShader = shader;
    }

    static void setDefaultProcGenShader(LILLIS::Shader shader)
    {
        procGenShader = shader;
    }

    static void initRenderData();
    static void shutdownRenderData();

    static void DrawUI(const Texture2D& texture, glm::vec2 position, int frame,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));

    static void DrawSprite(const Texture2D& texture, glm::vec2 position, float renderVal, int frame,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));

    static void DrawProcGen(std::vector<float>& verts, std::vector<float>& colors, int numObjects, RenderPrimitive prim, glm::mat4 camera);

private:
    static inline LILLIS::Shader procGenShader{};
    static inline LILLIS::Shader spriteShader{};
    static inline LILLIS::Shader uiShader{};
    static inline unsigned int quadVAO{}, ProcGenVBO[2]{}, ProcGenVAO{};;
    static inline unsigned int VBO[2]{};
};