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
    virtual void DoPostProcess(unsigned int previousColorBuffer);

    void AddPostProcess(LILLIS::Shader shader);
    void RemovePostProcess(LILLIS::Shader shader)
    {
        for (int i = 0; i < postProcessChain.size(); i++)
        {
            if (postProcessChain[i].ID == shader.ID)
            {
                postProcessChain.erase(postProcessChain.begin() + i);
                break;
            }
        }
    }

protected:
    std::vector<LILLIS::Shader> postProcessChain;
};



#endif //POSTPROCESSSEGMENT_H
