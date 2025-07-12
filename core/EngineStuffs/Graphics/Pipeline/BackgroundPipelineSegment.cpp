//
// Created by Somed on 6/29/2025.
//

#include "BackgroundPipelineSegment.h"

#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>

#include "EngineStuffs/Graphics/Texture.h"
#include "Utils/ResourceManager.h"

BackgroundPipelineSegment::BackgroundPipelineSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    this->shader = shader;
}

void BackgroundPipelineSegment::InitSegment()
{
        // configure VAO/VBO
    //Maybe swap to triangle strips for efficiency?

    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    FBOs.push_back(0);
    colorBuffers.push_back(0);

    //Generate frame buffer.
    glGenFramebuffers(1, FBOs.data());
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);

    //8 bit RGBA color buffer
    glGenTextures(1, colorBuffers.data());
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
    if (render_settings.pixelPerfect)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffers[0], 0);
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
}

void BackgroundPipelineSegment::PreRender()
{
    shader.Use();
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    glBindVertexArray(VAOs[0]);
    for (int i = 0; i < colorBuffers.size(); i++)
    {
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
}

std::vector<ColorBufferWrapper> BackgroundPipelineSegment::RenderBackgrounds(std::vector<BackgroundImage> &backgrounds, LILLIS::Camera& camera, int numBackgrounds)
{
    std::vector<ColorBufferWrapper> fbosRendered;

    if (backgrounds.empty())
    {
        return fbosRendered;
    }

    shader.SetMatrix4("projection", camera.projectionMatrix(render_settings.pixelPerfect));
    shader.SetVector4f("spriteColor", {1.0f, 1.0f, 1.0f, 1.0f});
    shader.SetFloat("renderValue", 15);
    shader.SetInteger("image", 0);

    if (deferredRender)
    {
        int maxBuffers;
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxBuffers);

        //Ensure max backgrounds hasn't been reached.
        if (numBackgrounds > maxBuffers)
        {
            std::cout << "Max buffers reached" << std::endl;
            return fbosRendered;
        }

        //Ensure enough buffers exist.
        while (colorBuffers.size() < numBackgrounds)
        {
            colorBuffers.push_back(0);
            //8 bit RGBA color buffer
            glGenTextures(1, &colorBuffers.back());
            glBindTexture(GL_TEXTURE_2D, colorBuffers.back());
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
            if (render_settings.pixelPerfect)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (colorBuffers.size() - 1), colorBuffers.back(), 0);
        }

        int colorAttachment = 0, currentBackground = backgrounds[0].data->layer;
        fbosRendered.emplace_back(false, backgrounds[0].data->layer, colorBuffers[colorAttachment]);

        for (int i = 0; i < backgrounds.size(); i++)
        {
            bool newAttachment = false;
            if (currentBackground < backgrounds[i].data->layer)
            {
                currentBackground = backgrounds[i].data->layer;
                colorAttachment++;
                newAttachment = true;
            }
            const GLenum item = GL_COLOR_ATTACHMENT0 + colorAttachment;
            glDrawBuffers(1, &item);
            RenderBackgroundImage(backgrounds[i]);
            if (newAttachment)
            {
                fbosRendered.emplace_back(false, backgrounds[i].data->layer, colorBuffers[colorAttachment]);
            }
        }

        return fbosRendered;
    }

    for (auto it : backgrounds)
    {
        RenderBackgroundImage(it);
    }

    fbosRendered.emplace_back(false, 0, colorBuffers[0]);
    return fbosRendered;
}


void BackgroundPipelineSegment::RenderBackgroundImage(BackgroundImage &background)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(background.currentPosition.x, -background.currentPosition.y, 0.0f));
    model = glm::scale(model, glm::vec3(background.data->imageSize, 1.0f));
    shader.SetMatrix4("model", model);

    const Texture2D texture = ResourceManager::GetTexture(background.data->image);
    const int frame = background.data->imageFrames[background.data->currentFrame];

    const glm::vec4 spriteQuad = texture.spriteLocations[frame];
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

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



