#ifndef TIMER_H
#define TIMER_H

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>
#include <time.h>
#include <iostream>

class Timer
{
    public:
        Timer();
        virtual ~Timer();

        GLvoid startTime();
        GLvoid stopTime();
        GLvoid resetTime();

        GLvoid currTime();
        GLvoid prevTime();

        GLfloat computeDeltaTime();
        GLfloat getTick();

    private:
        GLfloat _startTime;
        GLfloat _currTime;
        GLfloat _prevTime;
};

#endif // TIMER_H
