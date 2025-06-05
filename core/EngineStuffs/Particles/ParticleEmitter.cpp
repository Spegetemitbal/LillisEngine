//
// Created by Somed on 6/3/2025.
//

#include "ParticleEmitter.h"
#include "Utils/LilRandom.h"

#include <glm/ext/matrix_transform.hpp>

void ParticleEmitter::ProcessParticles(float deltaTime)
{
    if (maxParticles == 0)
    {
        return;
    }

    if (!isEnabled && numActive == 0)
    {
        return;
    }

    elapsedTime += deltaTime;
    while (elapsedTime >= spawnSpeed)
    {
        elapsedTime -= spawnSpeed;
        if (!particleActive[numActive] && isEnabled)
        {
            particleActive[numActive] = true;
            if (transform.Exists())
            {
                particlePositions[numActive] = transform->GlobalPosition();
            } else
            {
                particlePositions[numActive] = startPos;
            }

            //Randomized start pos
            if (spawnBounds != glm::vec2(0.0f))
            {
                particlePositions[numActive] += glm::vec2(LilRandom::randFloat(-spawnBounds.x, spawnBounds.x),
                    LilRandom::randFloat(-spawnBounds.y, spawnBounds.y));
            }

            particleVelocities[numActive] = {LilRandom::randFloat(minInitVelocity.x,maxInitVelocity.x),
            LilRandom::randFloat(minInitVelocity.y,maxInitVelocity.y),};
            particleAccelerations[numActive] = startAcceleration;
            particleLifetimes[numActive] = lifetime;

            particleScale[numActive] = {LilRandom::randFloat(minStartScale.x,maxStartScale.x),
            LilRandom::randFloat(minStartScale.y,maxStartScale.y),} * 0.5f;
            //particleColors[numActive] = startColor;
            if (numActive < maxParticles)
            {
                numActive++;
            }
        } else
        {
            break;
        }
    }

    for (unsigned int i = 0; i < numActive; i++)
    {
        particleLifetimes[i] -= deltaTime;
        if (particleLifetimes[i] <= 0)
        {
            //Kill particle, swap with last active, retry.
            particleActive[i] = false;
            numActive--;
            SwapParticles(i, numActive);
            i--;
            continue;
        }
        particleVelocities[i] += particleAccelerations[i] * deltaTime;
        particlePositions[i] += particleVelocities[i] * deltaTime;
    }
}

void ParticleEmitter::SetEffect(ParticleEffect *effect)
{
    this->effect = effect;
}

void ParticleEmitter::SetMaxParticles(unsigned int maxParticles)
{
    if (maxParticles == 0)
    {
        return;
    }
    this->maxParticles = maxParticles;

    particleActive.clear();
    particleActive.reserve(maxParticles);
    particlePositions.clear();
    particlePositions.reserve(maxParticles);
    particleVelocities.clear();
    particleVelocities.reserve(maxParticles);
    particleAccelerations.clear();
    particleAccelerations.reserve(maxParticles);
    particleLifetimes.clear();
    particleLifetimes.reserve(maxParticles);
    particleScale.clear();
    particleScale.reserve(maxParticles);
    //particleColors.clear();
    //particleColors.reserve(maxParticles);

    for (int i = 0; i < maxParticles; i++)
    {
        particleActive.push_back(false);
        particlePositions.emplace_back(startPos);
        particleVelocities.emplace_back(minInitVelocity);
        particleAccelerations.emplace_back(startAcceleration);
        particleLifetimes.emplace_back(lifetime);
        particleScale.emplace_back(minStartScale * 0.5f);
        //particleColors.emplace_back(startColor);
    }
}

void ParticleEmitter::SwapParticles(unsigned int i, unsigned int j)
{
    std::swap(particlePositions[i], particlePositions[j]);
    std::swap(particleVelocities[i], particleVelocities[j]);
    std::swap(particleAccelerations[i], particleAccelerations[j]);
    //std::swap(particleColors[i], particleColors[j]);
    std::swap(particleScale[i], particleScale[j]);
    float lifeA = particleLifetimes[i];
    bool activeA = particleActive[i];
    particleActive[i] = particleActive[j];
    particleActive[j] = activeA;
    particleLifetimes[i] = particleLifetimes[j];
    particleLifetimes[j] = lifeA;
}





