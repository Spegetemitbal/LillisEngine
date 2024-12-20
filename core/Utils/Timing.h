#pragma once

class Timing
{
public:
    Timing() = default;
    ~Timing() = default;

    static double timeScale;
    static bool Paused;

    static void SetFrameRate(int frameRate);
    static double GetRealFrameRate() {return realFrameRate;}

    static double getTotalTime() { return totalTime; }
    static double getUnscaledTime() { return unscaledTime; }
    static double getDeltaTime() { return deltaTime; }
    static double getUnscaledDeltaTime() { return unscaledDeltaTime; }
    static double getTimeSinceLevelStart() { return timeSinceLevelStart; }
    static double getRealTime() { return realTime; }
    static u64 getFrame() { return frame; }

private:
    static bool Initted;

    friend class Engine;
    static void SetTime();
    static void Init();
    static void Tick();

    static double realFrameRate;
    static u64 frame;

    static double totalTime;
    static double unscaledTime;
    static double deltaTime;
    static double unscaledDeltaTime;
    static double timeSinceLevelStart;

    static double realTime;
    static double frameStart;
};