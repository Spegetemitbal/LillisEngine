#include "Timers.h"
#include "Timing.h"

FrameTimer::FrameTimer(int numFrames)
{
    if (numFrames < 2) {
        mGoalFrame = 2;
    } else {
        mGoalFrame = numFrames;
    }
    mStartFrame = Timing::getFrame();
}

bool FrameTimer::check()
{
    if (ranOut)
    {
        return true;
    } else
    {
        if (Timing::getFrame() - mStartFrame >= mGoalFrame)
        {
            ranOut = true;
        }
        return ranOut;
    }
}

void FrameTimer::reset()
{
    ranOut = false;
    mStartFrame = Timing::getFrame();
}

//resets Timer when called.
void FrameTimer::setTimer(int numFrames)
{
    reset();
    mGoalFrame = numFrames;
}

Timer::Timer(double time, bool realTime)
{
    mRealTime = realTime;
    if (!realTime)
    {
        mStartTime = Timing::getTotalTime();
    } else
    {
        mStartTime = Timing::getRealTime();
    }
    mGoalTime = time;
}

bool Timer::check()
{
    if (ranOut)
    {
        return true;
    } else
    {
        if (!mRealTime)
        {
            if (Timing::getTotalTime() - mStartTime >= mGoalTime)
            {
                ranOut = true;
            }
        } else
        {
            if (Timing::getRealTime() - mStartTime >= mGoalTime)
            {
                ranOut = true;
            }
        }
        return ranOut;
    }
}

void Timer::reset()
{
    ranOut = false;
    if (!mRealTime)
    {
        mStartTime = Timing::getTotalTime();
    } else
    {
        mStartTime = Timing::getRealTime();
    }
}

//resets Timer when called.
void Timer::setTimer(double time)
{
    reset();
    mGoalTime = time;
}



