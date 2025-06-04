//
// Created by Somed on 5/4/2025.
//

#ifndef PROCGEN_H
#define PROCGEN_H

#include <glm/fwd.hpp>
#include "ProcGenData.h"

#include "Color.h"

class ProcGen {
public:

    ProcGen() = delete;

    //TODO: Set safety params on this for GLFW not initialized.
    void SetLineWidth(float width);
    void SetPointSize(float diameter);
    void DrawDebugPoint(glm::vec2 point, Color color);
    void DrawDebugLine(glm::vec2 from, glm::vec2 to, Color color);

    void Render(glm::mat4 camera);

    void InsertBatch(ProcGenBatch* batch)
    {
        if (batch != nullptr)
        {
            batches.push_back(batch);
        }
    }

    void RemoveBatch(ProcGenBatch* batch)
    {
        if (batch != nullptr)
        {
            batches.erase(std::remove(batches.begin(), batches.end(), batch));
        }
    }

    void ClearDebugObjects()
    {
        debugPoints.clear();
        debugLines.clear();
        debugLineColor.clear();
        debugPointColor.clear();
    }

    void ClearBatches()
    {
        batches.clear();
    }

    static ProcGen* createInstance(unsigned int numObjects);
    static ProcGen* getInstance();
    static void destroyInstance();
private:
    static ProcGen* instance;
    ProcGen(unsigned int numObjects);

    std::vector<float> debugPoints;
    std::vector<float> debugPointColor;
    std::vector<float> debugLines;
    std::vector<float> debugLineColor;

    std::vector<ProcGenBatch*> batches;

    ~ProcGen() = default;
};



#endif //PROCGEN_H
