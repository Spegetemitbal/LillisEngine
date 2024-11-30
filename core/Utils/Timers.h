//
// Created by Somed on 11/29/2024.
//

#ifndef TIMERS_H
#define TIMERS_H



DLLUSAGE class FrameTimer {
public:
    FrameTimer() = delete;
    //Minimum frames is 2
    FrameTimer(int numFrames);

    void setTimer(int numFrames);
    void reset();
    bool check();

    ~FrameTimer() = default;
private:
    u64 mStartFrame;
    int mGoalFrame;
    bool ranOut = false;
};

DLLUSAGE class Timer {
public:
    Timer() = delete;
    //Must be above 0
    Timer(double time, bool realTime = false);

    void setTimer(double time);
    void reset();
    bool check();

    ~Timer() = default;
private:
    double mStartTime;
    double mGoalTime;
    bool ranOut = false;
    bool mRealTime = false;
};



#endif //TIMERS_H
