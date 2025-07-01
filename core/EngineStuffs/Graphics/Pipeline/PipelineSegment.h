//
// Created by Somed on 6/16/2025.
//

#ifndef PIPELINESEGMENT_H
#define PIPELINESEGMENT_H
#include "EngineStuffs/Graphics/ProcGenData.h"
#include "EngineStuffs/Graphics/RenderSettings.h"
#include "EngineStuffs/Graphics/Shader.h"

struct ColorBufferWrapper
{
    bool doPostProcess = true;
    int depth = 0;
    unsigned int colorAttachment = 0;
};

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
    size_t GetNumVBOs() {return VBOs.size();}
    unsigned int GetVAO(int index) {return VAOs[index];}
    size_t GetNumVAOs() {return VAOs.size();}
    unsigned int GetFBO(int index) {return FBOs[index];}
    size_t GetNumFBOs() {return FBOs.size();}
    unsigned int GetRBO(int index) {return RBOs[index];}
    size_t GetNumRBOs() {return RBOs.size();}
    unsigned int GetColorBuffer(int index) {return colorBuffers[index];}
    size_t GetNumColorBuffers() {return colorBuffers.size();}
    unsigned int GetDepthBuffer(int index) {return depthBuffers[index];}
    size_t GetNumDepthBuffers() {return depthBuffers.size();}
    unsigned int GetStencilBuffer(int index) {return stencilBuffers[index];}
    size_t GetNumStencilBuffers() {return stencilBuffers.size();}

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
