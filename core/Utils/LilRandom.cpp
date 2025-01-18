//
// Created by Somed on 1/17/2025.
//

#include "LilRandom.h"

#include "Engine.h"

float LilRandom::randFloat(float minInclusive, float maxExclusive)
{
    if (!initialized)
    {
        GenerateSeed();
    }

    if (maxExclusive <= minInclusive)
    {
        return 0;
    }

    std::uniform_real_distribution<float> distrib(minInclusive, maxExclusive);
    return distrib(gen);
}

int LilRandom::randInt(int minInclusive, int maxExclusive)
{
    if (!initialized)
    {
        GenerateSeed();
    }

    if (maxExclusive <= minInclusive + 1)
    {
        return 0;
    }

    std::uniform_int_distribution distrib(minInclusive, maxExclusive);
    return distrib(gen);
}

void LilRandom::GenerateSeed()
{
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    initialized = true;
}
