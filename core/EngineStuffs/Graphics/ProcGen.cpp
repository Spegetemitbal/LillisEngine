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

void ProcGen::DrawLine(glm::vec2 from, glm::vec2 to, Color color)
{
    lines.insert(lines.end(), {from.x, from.y, to.x, to.y});
    lineColor.insert(lineColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
    lineColor.insert(lineColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
    numLines++;
}

void ProcGen::DrawPoint(glm::vec2 point, Color color)
{
    points.insert(points.end(), {point.x, point.y});
    pointColor.insert(pointColor.end(), {color.getRf(), color.getGf(), color.getBf(), color.getAf()});
    numPoints++;
}

ProcGen::ProcGen(unsigned int numObjects)
{
    points = std::vector<float>();
    points.reserve(2 * numObjects);
    lines = std::vector<float>();
    lines.reserve(4 * numObjects);
    pointColor = std::vector<float>();
    pointColor.reserve(4 * numObjects);
    lineColor = std::vector<float>();
    lineColor.reserve(4 * numObjects);
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
    if (!points.empty())
    {
        SpriteRenderer::DrawProcGen(points, pointColor,numPoints, false, camera);
    }
    //Draw lines
    if (!lines.empty())
    {
        SpriteRenderer::DrawProcGen(lines, lineColor,numLines, true,camera);
    }
}









