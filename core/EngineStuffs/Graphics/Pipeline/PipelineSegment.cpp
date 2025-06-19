//
// Created by Somed on 6/16/2025.
//

#include "PipelineSegment.h"
#include <glad/gl.h>

PipelineSegment::PipelineSegment(RenderSettings render_settings)
{
    this->render_settings = render_settings;
}

PipelineSegment::~PipelineSegment()
{
    DestroyBuffersAndArrays();
}


void PipelineSegment::PreRender()
{
    if (hasBlending)
    {
        glEnable(GL_BLEND);
    } else
    {
        glDisable(GL_BLEND);
    }

    if (hasDepthTesting)
    {
        glEnable(GL_DEPTH_TEST);
    } else
    {
        glDisable(GL_DEPTH_TEST);
    }

    if (hasFaceCulling)
    {
        glEnable(GL_CULL_FACE);
    } else
    {
        glDisable(GL_CULL_FACE);
    }
}

void PipelineSegment::PostRender()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PipelineSegment::SetShader(LILLIS::Shader shader)
{
    this->shader = shader;
}

void PipelineSegment::ClearBuffer(bool depthClear, bool stencilClear)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (depthClear)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    if (stencilClear)
    {
        glClear(GL_STENCIL_BUFFER_BIT);
    }
}


void PipelineSegment::DestroyBuffersAndArrays()
{
    glDeleteBuffers((GLsizei)VBOs.size(), VBOs.data());
    glDeleteVertexArrays((GLsizei)VAOs.size(), VAOs.data());
    glDeleteFramebuffers((GLsizei)FBOs.size(), FBOs.data());
    glDeleteRenderbuffers((GLsizei)RBOs.size(), RBOs.data());
    VBOs.clear();
    VAOs.clear();
    FBOs.clear();
    RBOs.clear();
}











