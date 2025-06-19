//
// Created by Somed on 6/16/2025.
//

#ifndef PARTICLEPIPELINESEGMENT_H
#define PARTICLEPIPELINESEGMENT_H
#include "PipelineSegment.h"
#include "EngineStuffs/MemoryManagement/MemoryPool.h"


class ParticleEmitter;

class ParticlePipelineSegment : public PipelineSegment
{
public:
    ParticlePipelineSegment(RenderSettings render_settings, LILLIS::Shader shader);
    ParticlePipelineSegment() = delete;

    void InitSegment() override;
    void PreRender() override;
    virtual void DoStep(ActiveTracker<ParticleEmitter*>& emitters, unsigned int lastEmitter, float upSprite, float downSprite,
        glm::mat4 camera);
protected:
    virtual void RenderParticles(ParticleEmitter& emitter, glm::mat4 camera, float upSprite, float invDist, bool doRenderAxis);
};



#endif //PARTICLEPIPELINESEGMENT_H
