//
// Created by Somed on 6/16/2025.
//

#include "PostProcessSegment.h"

#include "EngineStuffs/Graphics/Parallax.h"
#include "glad/gl.h"


PostProcessSegment::PostProcessSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    this->shader = shader;
    perLayerShader = this->shader;
    finalShader = this->shader;
}

void PostProcessSegment::InitSegment()
{
    FBOs.push_back(0);
    colorBuffers.push_back(0);
    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    //Generate frame buffer.
    glGenFramebuffers(1, &FBOs[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);

    //8 bit RGBA color buffer
    glGenTextures(1, &colorBuffers[0]);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.windowWidth, (GLsizei)render_settings.windowHeight);
    if (render_settings.pixelPerfect)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffers[0], 0);

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer incomplete: %d", fboStatus);
        return;
    }

    glGenVertexArrays(1, &VAOs[0]);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PostProcessSegment::PreRender()
{
    glBindVertexArray(VAOs[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, (GLsizei)render_settings.windowWidth, (GLsizei)render_settings.windowHeight);
}

void PostProcessSegment::DoPostProcess(std::vector<ColorBufferWrapper> wrappers, int numSprWrappers)
{
    glm::mat4 view = glm::ortho(0.0f, (float)render_settings.windowWidth, (float)render_settings.windowHeight, 0.0f, 0.0f, 10.0f);
    //view[1][1] *= -1.0f;

    //Run default.
    perLayerShader.Use();
    glm::mat4 model = glm::mat4(1.0f);

    int xScale = (int)render_settings.windowWidth;
    int yScale = (int)render_settings.windowHeight;
    if (render_settings.pixelPerfect)
    {
        xScale += 4;
        yScale += 4;
        //model = glm::translate(model, glm::vec3(Parallax::getCameraOffset(), 0.0f));
    }

    model = glm::scale(model, glm::vec3(render_settings.windowWidth, render_settings.windowHeight, 1.0f));

    perLayerShader.SetMatrix4("model", model);
    perLayerShader.SetInteger("image", 0);
    perLayerShader.SetMatrix4("projection", view);
    shader.SetMatrix4("model", model);
    shader.SetInteger("image", 0);
    shader.SetMatrix4("projection", view);

    bool perLayerUse = true;

    glActiveTexture(GL_TEXTURE0);

    //First draw all wrappers to colorbuffer.
    for (int i = 0; i < wrappers.size(); i++)
    {
        if (perLayerUse)
        {
            if (!wrappers[i].doPerLayerPostProcess)
            {
                perLayerUse = false;
                shader.Use();
            }
        } else
        {
            if (wrappers[i].doPerLayerPostProcess)
            {
                perLayerUse = true;
                perLayerShader.Use();
            }
        }
        glBindTextureUnit(0, wrappers[i].colorAttachment);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    finalShader.Use();
    finalShader.SetMatrix4("model", model);
    finalShader.SetInteger("image", 0);
    finalShader.SetMatrix4("projection", view);

    //Final draw to backbuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTextureUnit(0, colorBuffers[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
