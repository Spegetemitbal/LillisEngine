//
// Created by Somed on 5/4/2025.
//

#include "ProcGen.h"

#include <glad/gl.h>

#include "SpriteRenderer.h"

ProcGen* ProcGen::instance = nullptr;

ProcGen *ProcGen::createInstance(unsigned int numObjects)
{
    if (numObjects < 20)
    {
        numObjects = 20;
    }

    if (instance == nullptr)
    {
        instance = DBG_NEW ProcGen(numObjects);
    }
    return instance;
}

ProcGen *ProcGen::getInstance()
{
    if (instance == nullptr)
    {
        createInstance(20);
    }
    return instance;
}

void ProcGen::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void ProcGen::DrawDebugLine(glm::vec2 from, glm::vec2 to, Color color)
{
    debugLines.insert(debugLines.end(), {from.x, from.y, to.x, to.y});
    debugLineColor.insert(debugLineColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
    debugLineColor.insert(debugLineColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
}

void ProcGen::DrawDebugPoint(glm::vec2 point, Color color)
{
    debugPoints.insert(debugPoints.end(), {point.x, point.y});
    debugPointColor.insert(debugPointColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
}

ProcGen::ProcGen(unsigned int numObjects)
{
    debugPoints = std::vector<float>();
    debugPoints.reserve(2 * numObjects);
    debugLines = std::vector<float>();
    debugLines.reserve(4 * numObjects);
    debugPointColor = std::vector<float>();
    debugPointColor.reserve(4 * numObjects);
    debugLineColor = std::vector<float>();
    debugLineColor.reserve(4 * numObjects);
}


void ProcGen::SetLineWidth(float width)
{

    if (width <= 0)
    {
        width = 1;
    }
    glLineWidth(width);
}

void ProcGen::SetPointSize(float diameter)
{
    if (diameter <= 0)
    {
        diameter = 1;
    }
    glPointSize(diameter);
}

void ProcGen::Render(glm::mat4 camera)
{
    //Draw points
    if (!debugPoints.empty())
    {
        SpriteRenderer::DrawProcGen(debugPoints, debugPointColor,(int)debugPoints.size() / 2, PRIMITIVE_POINT, camera);
    }
    //Draw lines
    if (!debugLines.empty())
    {
        SpriteRenderer::DrawProcGen(debugLines, debugLineColor,(int)debugLines.size() / 2, PRIMITIVE_LINE,camera);
    }
    //Draw batches
    for (int i = 0; i < batches.size(); i++)
    {
        SpriteRenderer::DrawProcGen(batches[i]->vertices, batches[i]->colors, (int)batches[i]->vertices.size() / 2, PRIMITIVE_TRIANGLE, camera);
    }
}









