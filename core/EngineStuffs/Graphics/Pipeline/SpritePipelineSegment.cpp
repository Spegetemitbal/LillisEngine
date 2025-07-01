//
// Created by Somed on 6/16/2025.
//

#include "SpritePipelineSegment.h"
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>

#include "Utils/ResourceManager.h"
#include "../BackgroundManager.h"

SpritePipelineSegment::SpritePipelineSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    SetShader(shader);
}

void SpritePipelineSegment::InitSegment()
{
    // configure VAO/VBO
    //Maybe swap to triangle strips for efficiency?

    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    FBOs.push_back(0);
    colorBuffers.push_back(0);
    depthBuffers.push_back(0);

    //Generate frame buffer.
    glGenFramebuffers(1, FBOs.data());
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);

    //8 bit RGBA color buffer
    glGenTextures(1, colorBuffers.data());
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, render_settings.resolutionWidth, render_settings.resolutionHeight);

    //Make depth buffer.
    glGenTextures(1, depthBuffers.data());
    glBindTexture(GL_TEXTURE_2D, depthBuffers[0]);
    //Maybe use a higher precision buffer if issues are arising.
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, render_settings.resolutionWidth, render_settings.resolutionHeight);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffers[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffers[0],0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCreateVertexArrays(1, VAOs.data());
    glGenBuffers(2, VBOs.data());

    glBindVertexArray(VAOs[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    float positions[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    float texCoords[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    //float rendValue = 0.0f;

    //load positions
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Load texture coordinates.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);
    glViewport(0, 0, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);
}

void SpritePipelineSegment::PreRender()
{
    shader.Use();
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


std::vector<ColorBufferWrapper> SpritePipelineSegment::DoStep(std::vector<Sprite *> &sprites, unsigned int lastSprite, std::vector<TileMap> &tile_maps, glm::mat4 camera)
{
    for (int i = 0; i < sprites.size(); i++)
    {
        Sprite* spr = sprites[i];
        if (spr->image.empty())
        {
            throw;
        }
        Texture2D tex = ResourceManager::GetTexture(spr->image);
        RenderSprite(tex, spr->getRenderLocation(), spr->getRenderValue(), (int)spr->frame, camera,
            spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
    }

    for (auto & tMap: tile_maps)
    {
        if (tMap.active)
        {
            glm::vec2 renderSize = tMap.getTileSize();
            for (int i = 0; i < tMap.tilesToRender.size(); i++)
            {
                TileLoc t = tMap.tilesToRender[i];
                std::string img = tMap.getImageFromIndex(t.tile);
                unsigned int frm = tMap.getFrameFromIndex(t.tile);
                if (img.empty())
                {
                    throw;
                }
                Texture2D tex = ResourceManager::GetTexture(img);
                RenderSprite(tex, t.worldPos, t.zVal, (int)frm, camera,
                    renderSize, 0);
            }
        }
    }

    //TODO add possible splitting.
    return {ColorBufferWrapper(true, 0, colorBuffers[0])};
}

void SpritePipelineSegment::RenderSprite(const Texture2D &texture, glm::vec2 position, float renderVal, int frame, glm::mat4 camera, glm::vec2 size, float rotate, glm::vec3 color)
{
    size *= (float)render_settings.pixelsPerUnit;
    position *= (float)render_settings.pixelsPerUnit;
    // prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    //glm::vec2 flippedPosition = {position.x, -position.y};
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    glm::vec4 newColor = glm::vec4(color, 1.0f);

    shader.SetMatrix4("model", model);
    shader.SetVector4f("spriteColor", newColor);
    shader.SetFloat("renderValue", renderVal);
    shader.SetInteger("image", 0);
    shader.SetMatrix4("projection", camera);

    glm::vec4 spriteQuad = texture.spriteLocations[frame];
    float texCoords[] =
    {
        spriteQuad.x, spriteQuad.w,
        spriteQuad.y, spriteQuad.z,
        spriteQuad.x, spriteQuad.z,

        spriteQuad.x, spriteQuad.w,
        spriteQuad.y, spriteQuad.w,
        spriteQuad.y, spriteQuad.z
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


