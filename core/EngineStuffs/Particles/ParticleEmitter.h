//
// Created by Somed on 6/3/2025.
//

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H
#include <glm/vec4.hpp>

#include "ParticleEffect.h"
#include "EngineStuffs/Transform.h"

enum ParticleChangeOverLifetime
{
    NONE,
    LINEARINCREASE,
    LINEARDECREASE,
    QUADRATICSLOWINCREASE,
    QUADRATICFASTINCREASE,
    QUADRATICSLOWDECREASE,
    QUADRATICFASTDECREASE
};

struct ParticleEmitterData
{
    glm::vec2 startPos = {};
    glm::vec2 spawnBounds = {};
    glm::vec2 minInitVelocity = {};
    glm::vec2 maxInitVelocity = {};
    glm::vec2 startAcceleration = {};
    glm::vec2 minStartScale = {1,1};
    glm::vec2 maxStartScale = {1,1};
    glm::vec4 startColor = {1,1,1,1};
    glm::vec4 endColor = {1,1,1,1};
    float lifetime = 0.5f;
    float spawnSpeed = 1.0f;
};

class ParticleEmitter : public Component
{
public:

    ParticleEmitter() = default;
    ~ParticleEmitter() = default;

    glm::vec2 startPos = {};
    glm::vec2 spawnBounds = {};
    LilObj<Transform> transform;
    glm::vec2 minInitVelocity = {};
    glm::vec2 maxInitVelocity = {};
    glm::vec2 startAcceleration = {};
    glm::vec2 minStartScale = {1,1};
    glm::vec2 maxStartScale = {1,1};
    glm::vec4 startColor = {1,1,1,1};
    glm::vec4 endColor = {1,1,1,1};
    float lifetime = 0.5f;
    float spawnSpeed = 1.0f;

    void SetEffect(ParticleEffect* effect);
    void SetMaxParticles(unsigned int maxParticles);
    //Make sure to still call this on un-enabled particleSystems.
    void ProcessParticles(float deltaTime);
    void DisableAllParticles()
    {
        for (auto && i : particleActive)
        {
            i = false;
        }
    }

    bool particleCollision = false;

private:

    friend class SpriteRenderer;
    friend class PhysicsSystem;

    ParticleEffect* effect = nullptr;
    unsigned int maxParticles = 0;
    unsigned int numActive = 0;
    float elapsedTime = 0.0f;

    void SwapParticles(unsigned int i, unsigned int j);

    //Particle Data
    std::vector<bool> particleActive = std::vector<bool>();
    std::vector<glm::vec2> particlePositions = std::vector<glm::vec2>();
    std::vector<glm::vec2> particleVelocities = std::vector<glm::vec2>();
    std::vector<glm::vec2> particleAccelerations = std::vector<glm::vec2>();
    std::vector<float> particleLifetimes = std::vector<float>();
    std::vector<glm::vec2> particleScale = std::vector<glm::vec2>();
    //std::vector<glm::vec4> particleColors = std::vector<glm::vec4>();
};



#endif //PARTICLEEMITTER_H
