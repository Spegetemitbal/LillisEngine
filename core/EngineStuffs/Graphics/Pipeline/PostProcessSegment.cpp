//
// Created by Somed on 6/16/2025.
//

#include "PostProcessSegment.h"

#include "glad/gl.h"


PostProcessSegment::PostProcessSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    this->shader = shader;
}

void PostProcessSegment::InitSegment()
{
    FBOs.push_back(0);
    colorBuffers.push_back(0);
    VAOs.push_back(0);

    //Generate frame buffer.
    glGenFramebuffers(1, &FBOs[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);

    //8 bit RGBA color buffer
    glGenTextures(1, &colorBuffers[0]);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionWidth, (GLsizei)render_settings.resolutionHeight);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffers[0], 0);

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer incomplete: %d", fboStatus);
        return;
    }

    glGenVertexArrays(1, &VAOs[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessSegment::PreRender()
{
    glBindVertexArray(VAOs[0]);
    glViewport(0, 0, (GLsizei)render_settings.windowWidth, (GLsizei)render_settings.windowHeight);
}

void PostProcessSegment::DoPostProcess(unsigned int previousColorBuffer)
{
    //Run all added postProcesses.
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    for (int i = 0; i < postProcessChain.size(); i++)
    {
        LILLIS::Shader& shad = postProcessChain[i];
        if (i == 0)
        {
            glBindTextureUnit(0, previousColorBuffer);
        } else
        {
            glBindTextureUnit(0, colorBuffers[0]);
        }
        shad.Use();
        shad.SetInteger("_ColorBuffer", 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    //Run default.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (postProcessChain.empty())
    {
        glBindTextureUnit(0, previousColorBuffer);
    } else
    {
        glBindTextureUnit(0, colorBuffers[0]);
    }

    shader.Use();
    shader.SetInteger("_ColorBuffer", 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void PostProcessSegment::AddPostProcess(LILLIS::Shader shader)
{
    postProcessChain.push_back(shader);
}
