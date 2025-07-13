//
// Created by Somed on 6/16/2025.
//

#ifndef POSTPROCESSSEGMENT_H
#define POSTPROCESSSEGMENT_H

#include "PipelineSegment.h"

class PostProcessSegment : public PipelineSegment
{
public:
    PostProcessSegment(RenderSettings render_settings, LILLIS::Shader shader);
    PostProcessSegment() = delete;

    void InitSegment() override;
    void PreRender() override;
    virtual void DoPostProcess(std::vector<ColorBufferWrapper> wrappers, int numSprWrappers);

    void SetPerLayerShader(LILLIS::Shader shader)
    {
        perLayerShader = shader;
    }
    void SetFinalShader(LILLIS::Shader shader)
    {
        finalShader = shader;
    }

protected:
    LILLIS::Shader perLayerShader;
    LILLIS::Shader finalShader;
};



#endif //POSTPROCESSSEGMENT_H
