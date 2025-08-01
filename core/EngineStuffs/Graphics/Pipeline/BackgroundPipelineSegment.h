//
// Created by Somed on 6/29/2025.
//

#ifndef BACKGROUNDPIPELINESEGMENT_H
#define BACKGROUNDPIPELINESEGMENT_H
#include "PipelineSegment.h"
#include "../BackgroundManager.h"

class BackgroundPipelineSegment : public PipelineSegment {
public:
    BackgroundPipelineSegment(RenderSettings render_settings, LILLIS::Shader shader);

    void InitSegment() override;
    void PreRender() override;

    virtual std::vector<ColorBufferWrapper> RenderBackgrounds(std::vector<BackgroundImage>& backgrounds, LILLIS::Camera& camera, int numBackgrounds);
    bool deferredRender = false;
    bool doParallax = false;

    bool doPostProcess = true;
protected:
    virtual void RenderBackgroundImage(BackgroundImage& background);
};



#endif //BACKGROUNDPIPELINESEGMENT_H
