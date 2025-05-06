//
// Created by Somed on 5/4/2025.
//

#ifndef PROCGEN_H
#define PROCGEN_H

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "Color.h"

class ProcGen {
public:

    ProcGen() = delete;

    //TODO: Set safety params on this for GLFW not initialized.
    void SetLineWidth(float width);
    void SetPointSize(float diameter);
    void DrawPoint(glm::vec2 point, Color color);
    void DrawLine(glm::vec2 from, glm::vec2 to, Color color);

    void Render(glm::mat4 camera);

    void ClearObjects()
    {
        points.clear();
        lines.clear();
        lineColor.clear();
        pointColor.clear();
        numPoints = 0;
        numLines = 0;
    }

    static ProcGen* createInstance(unsigned int numObjects);
    static ProcGen* getInstance();
    static void destroyInstance();
private:
    static ProcGen* instance;
    ProcGen(unsigned int numObjects);

    std::vector<float> points;
    std::vector<float> pointColor;
    std::vector<float> lines;
    std::vector<float> lineColor;
    int numPoints = 0;
    int numLines = 0;

    ~ProcGen() = default;
};



#endif //PROCGEN_H
