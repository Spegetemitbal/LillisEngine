//
// Created by Somed on 6/16/2025.
//

#ifndef PIPELINESEGMENT_H
#define PIPELINESEGMENT_H
#include "EngineStuffs/Graphics/ProcGenData.h"
#include "EngineStuffs/Graphics/RenderSettings.h"
#include "EngineStuffs/Graphics/Shader.h"

class PipelineSegment {
public:

    PipelineSegment(RenderSettings render_settings);
    PipelineSegment() = delete;

    virtual ~PipelineSegment();

    void SetShader(LILLIS::Shader shader);
    virtual void InitSegment() = 0;

    void DestroyBuffersAndArrays();

    LILLIS::Shader& GetShader() {return shader;};

    //In inherited items, give interface for inserting specific vertex or uniform data.

    unsigned int GetVBO(int index) {return VBOs[index];}
    unsigned int GetVAO(int index) {return VAOs[index];}
    unsigned int GetFBO(int index) {return FBOs[index];}
    unsigned int GetRBO(int index) {return RBOs[index];}
    unsigned int GetColorBuffer(int index) {return colorBuffers[index];}
    unsigned int GetDepthBuffer(int index) {return depthBuffers[index];}
    unsigned int GetStencilBuffer(int index) {return stencilBuffers[index];}

    virtual void PreRender();
    void PostRender();

protected:

    void ClearBuffer(bool depthClear = false, bool stencilClear = false);

    bool hasDepthTesting = false, hasBlending = false, hasAntiAliasing = false, hasFaceCulling = false;

    LILLIS::Shader shader;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> FBOs;
    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> RBOs;
    std::vector<unsigned int> colorBuffers;
    std::vector<unsigned int> depthBuffers;
    std::vector<unsigned int> stencilBuffers;
    RenderSettings render_settings;
};



#endif //PIPELINESEGMENT_H
