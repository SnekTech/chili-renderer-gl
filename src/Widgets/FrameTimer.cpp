//
// Created by syf on 2023/3/10.
//

#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
{
    last = steady_clock::now();
}

float FrameTimer::Mark()
{
    const auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    return frameTime.count();
}