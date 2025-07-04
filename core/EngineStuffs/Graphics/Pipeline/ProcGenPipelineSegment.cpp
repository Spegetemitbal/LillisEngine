//
// Created by Somed on 6/16/2025.
//

#include "ProcGenPipelineSegment.h"
#include <glad/gl.h>

ProcGenPipelineSegment::ProcGenPipelineSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    SetShader(shader);
}

void ProcGenPipelineSegment::InitSegment()
{
    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    glGenVertexArrays((GLsizei)VAOs.size(), VAOs.data());
    glGenBuffers((GLsizei)VBOs.size(), VBOs.data());
}

void ProcGenPipelineSegment::PreRender()
{
    shader.Use();
    glBindVertexArray(VAOs[0]);
    glDisable(GL_DEPTH_TEST);
}

void ProcGenPipelineSegment::RenderProcGen(std::vector<float> &verts, std::vector<float> &colors, int numObjects, RenderPrimitive prim, LILLIS::Camera& camera)
{
    switch (prim)
    {
        case PRIMITIVE_POINT:
            //Wheeee
                break;
        case PRIMITIVE_LINE:
            numObjects /= 2;
        break;
        case PRIMITIVE_TRIANGLE:
            numObjects /= 3;
        break;
        default:
            std::cout << "Unsupported primitive type" << std::endl;
        return;
    }


    shader.SetMatrix4("_projection", camera.projectionMatrix());
    shader.SetFloat("_ppu", (float)render_settings.pixelsPerUnit);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Set vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)verts.size() * (GLsizeiptr)sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,  2 * sizeof(float), (void*)0);

    //Set Colors.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)colors.size() * (GLsizeiptr)sizeof(float), colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,  4 * sizeof(float), (void*)0);

    if (prim == RenderPrimitive::PRIMITIVE_LINE)
    {
        glDrawArrays(GL_LINES, 0, numObjects);
    } else if (prim == RenderPrimitive::PRIMITIVE_POINT)
    {
        glDrawArrays(GL_POINTS, 0, numObjects);
    } else
    {
        glDrawArrays(GL_TRIANGLES, 0, numObjects);
    }
}



