#include "Timer.h"

Timer::Timer()
{
    _startTime = 0.0f;
    _currTime  = 0.0f;
    _prevTime  = 0.0f;
}

Timer::~Timer()
{
    //dtor
}

GLvoid Timer::startTime()
{

}

GLvoid Timer::stopTime()
{

}

GLvoid Timer::resetTime()
{
    _startTime = clock();
}


GLfloat Timer::computeDeltaTime()
{
    GLfloat _deltaTime = ((_currTime - _prevTime) / 1000) * 60;

    if (_deltaTime > 0.7f) {
       _deltaTime = 0.7f;
    }

    return _deltaTime;
}

GLfloat Timer::getTick()
{
    return clock() - _startTime;
}

GLvoid Timer::currTime()
{
    this->_currTime = clock();
}

GLvoid Timer::prevTime()
{
    this->_prevTime = _currTime;
}

