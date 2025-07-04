//
// Created by Somed on 6/16/2025.
//

#ifndef PROCGENPIPELINESEGMENT_H
#define PROCGENPIPELINESEGMENT_H
#include "PipelineSegment.h"


class ProcGenPipelineSegment : public PipelineSegment
{
public:
    ProcGenPipelineSegment(RenderSettings render_settings, LILLIS::Shader shader);
    ProcGenPipelineSegment() = delete;

    void InitSegment() override;
    void PreRender() override;
    virtual void RenderProcGen(std::vector<float>& verts, std::vector<float>& colors, int numObjects, RenderPrimitive prim, LILLIS::Camera& camera);
};



#endif //PROCGENPIPELINESEGMENT_H
