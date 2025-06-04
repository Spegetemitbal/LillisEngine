//
// Created by Somed on 3/13/2025.
//

#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H
#include <numeric>
#include <tuple>

struct RenderSettings
{
    unsigned int resolutionWidth = 640, resolutionHeight = 360;
    unsigned int windowWidth = 640, windowHeight = 360;
    unsigned int pixelsPerUnit = 40;
    bool fullScreen = false;
    bool ppuIsPercentage = false;

    RenderSettings()
    = default;

    RenderSettings(unsigned int resolutionWidth, unsigned int resolutionHeight,
        unsigned int windowWidth, unsigned int windowHeight, unsigned int pixelsPerUnit, bool fullScreen = false)
    {
        this->resolutionWidth = resolutionWidth;
        this->resolutionHeight = resolutionHeight;
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        this->fullScreen = fullScreen;
        this->pixelsPerUnit = pixelsPerUnit;
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

    void SetPPUasPercentage(unsigned int percentage, bool byWidth = true)
    {
        if (percentage > 100 || percentage < 1)
        {
            std::cout << "Invalid percentage: " << percentage << std::endl;
            return;
        }
        ppuIsPercentage = true;
        if (byWidth)
        {
            pixelsPerUnit = resolutionWidth * (percentage / 100);
        } else
        {
            pixelsPerUnit = resolutionHeight * (percentage / 100);
        }
    }
};

#endif //RENDERSETTINGS_H
