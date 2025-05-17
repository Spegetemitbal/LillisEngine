#include "Timing.h"
#include <GLFW/glfw3.h>

bool Timing::Initted = false;
bool Timing::Paused = false;
double Timing::timeScale = 1;
double Timing::totalTime = 0;
double Timing::unscaledTime = 0;
double Timing::realTime = 0;
double Timing::frameStart = 0;
double Timing::deltaTime = 0;
double Timing::unscaledDeltaTime = 0;
double Timing::timeSinceLevelStart = 0;

double Timing::realFrameRate = 0.02;
u64 Timing::frame = 0;

//Call once, after glfwinit
void Timing::Init() {
    if (!Timing::Initted) {
        Initted = true;
        totalTime = glfwGetTime();
        unscaledTime = totalTime;
        realTime = unscaledTime;
        frameStart = -1;
    }
}

void Timing::SetTime()
{
    realTime = glfwGetTime();
}


void Timing::Tick() {


    SetTime();
    double preDelta = realTime - frameStart;
    if (timeScale <= 0)
    {
        timeScale = 1;
    }
    frameStart = realTime;
    if (!Paused)
    {
        frame++;
        unscaledDeltaTime = preDelta;
        deltaTime = preDelta * timeScale;

        totalTime += deltaTime;
        unscaledTime += unscaledDeltaTime;
        frameLag += deltaTime;

        timeSinceLevelStart += unscaledDeltaTime;
    }
}


void Timing::SetFrameRate(int frameRate)
{
    realFrameRate = 1.0 / frameRate;
    fixedUpdateTime = realFrameRate;
}
