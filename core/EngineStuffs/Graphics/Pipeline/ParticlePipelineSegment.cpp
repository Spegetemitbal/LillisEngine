//
// Created by Somed on 6/16/2025.
//

#include "ParticlePipelineSegment.h"
#include "../../Particles/ParticleEmitter.h"
#include "EngineStuffs/Graphics/RenderOrder.h"
#include <glad/gl.h>

#include "EngineStuffs/Graphics/SpriteRenderer.h"
#include "Utils/ResourceManager.h"

ParticlePipelineSegment::ParticlePipelineSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    SetShader(shader);
}

void ParticlePipelineSegment::InitSegment()
{
    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    glCreateVertexArrays(1, VAOs.data());
    glGenBuffers(3, VBOs.data());
}

void ParticlePipelineSegment::PreRender()
{
    shader.Use();
    glBindVertexArray(VAOs[0]);
}

void ParticlePipelineSegment::DoStep(ActiveTracker<ParticleEmitter *> &emitters, unsigned int lastEmitter, float upSprite, float downSprite,
        glm::mat4 camera)
{
    bool doRenderAxis = RenderOrder::GetRenderAxis() != glm::vec2(0);
    float invDist = 1 / (upSprite - downSprite);
    for (int i = 0; i < lastEmitter; i++)
    {
        if (emitters[i]->GetActive())
        {
            RenderParticles(*emitters[i], camera, upSprite, invDist, doRenderAxis);
        }
    }
}

void ParticlePipelineSegment::RenderParticles(ParticleEmitter &emitter, glm::mat4 camera, float upSprite, float invDist, bool doRenderAxis)
{
    if (emitter.numActive < 0)
    {
        return;
    }

    shader.SetMatrix4("projection", camera);
    shader.SetFloat("_ppu", (float)render_settings.pixelsPerUnit);
    shader.SetFloat("renderValue", emitter.renderVal);
    if (doRenderAxis && !emitter.ignoreRenderAxis)
    {
        shader.SetFloat("layerDif", emitter.layerDif);
    } else
    {
        shader.SetFloat("layerDif", 0);
    }
    shader.SetFloat("upSprite", upSprite);
    shader.SetFloat("invDist", invDist);
    shader.SetInteger("image", 0);
    shader.SetVector4f("startColor", emitter.startColor);
    shader.SetVector4f("endColor", emitter.endColor);
    shader.SetFloat("maxTime", emitter.lifetime);
    Texture2D texture = ResourceManager::GetTexture(emitter.effect->image);
    shader.SetVector4f("TexQuad", texture.spriteLocations[emitter.effect->frame]);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //Set vertices
    std::vector<glm::vec2> positions;
    positions.assign(emitter.particlePositions.begin(), emitter.particlePositions.begin() + emitter.numActive);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)emitter.numActive * (GLsizeiptr)sizeof(glm::vec2), positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,  2 * sizeof(float), (void*)0);

    //Set halfwidths.
    std::vector<glm::vec2> halfwidths;
    halfwidths.assign(emitter.particleScale.begin(), emitter.particleScale.begin() + emitter.numActive);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)emitter.numActive * (GLsizeiptr)sizeof(glm::vec2), halfwidths.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  2 * sizeof(float), (void*)0);

    //Set durations.
    std::vector<float> durations;
    durations.assign(emitter.particleLifetimes.begin(), emitter.particleLifetimes.begin() + emitter.numActive);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)emitter.numActive * (GLsizeiptr)sizeof(float), durations.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (void*)0);

    glDrawArrays(GL_POINTS, 0, (GLsizei)emitter.numActive);
}





