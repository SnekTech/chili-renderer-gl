//
// Created by syf on 2023/3/10.
//

#ifndef CHILI_RENDERER_GL_FRAMETIMER_H
#define CHILI_RENDERER_GL_FRAMETIMER_H

#include <chrono>

class FrameTimer
{
public:
    FrameTimer();
    float Mark();
private:
    std::chrono::steady_clock::time_point  last;
};

#endif //CHILI_RENDERER_GL_FRAMETIMER_H
