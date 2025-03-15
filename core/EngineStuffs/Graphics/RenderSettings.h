//
// Created by Somed on 3/13/2025.
//

#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H
#include <numeric>
#include <tuple>

struct RenderSettings
{
    unsigned int resolutionWidth = 0, resolutionHeight = 0;
    unsigned int windowWidth = 0, windowHeight = 0;
    bool fullScreen = false;

    RenderSettings()
    = default;

    RenderSettings(unsigned int resolutionWidth, unsigned int resolutionHeight,
        unsigned int windowWidth, unsigned int windowHeight, bool fullScreen = false)
    {
        this->resolutionWidth = resolutionWidth;
        this->resolutionHeight = resolutionHeight;
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        this->fullScreen = fullScreen;
    }

    void ScaleAll(unsigned int scalar, bool increase)
    {
        ScaleResolution(scalar, increase);
        ScaleWindow(scalar, increase);
    }
    void ScaleResolution(unsigned int scalar, bool increase)
    {
        if (scalar < 1)
        {
            return;
        }

        if (increase)
        {
            resolutionWidth *= scalar;
            resolutionHeight *= scalar;
        } else
        {
            resolutionWidth /= scalar;
            resolutionHeight /= scalar;
        }
    }
    void ScaleWindow(unsigned int scalar, bool increase)
    {
        if (scalar < 1)
        {
            return;
        }

        if (increase)
        {
            windowWidth *= scalar;
            windowHeight *= scalar;
        } else
        {
            windowWidth /= scalar;
            windowHeight /= scalar;
        }
    }

    std::tuple<unsigned int, unsigned int> aspectRatio() const
    {
        unsigned int GCD = std::gcd(resolutionWidth, resolutionHeight);
        return { resolutionWidth / GCD, resolutionHeight / GCD };
    }
};

#endif //RENDERSETTINGS_H
