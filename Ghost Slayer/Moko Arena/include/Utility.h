#ifndef UTILITY_H
#define UTILITY_H

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>

enum Phase
{
    NONE = 0,
    PLAY,
    HOW_TO_PLAY,
    LOSE,
    EXIT
};

struct Coordinate
{
    GLfloat _xPos;
    GLfloat _yPos;

    Coordinate(GLfloat, GLfloat);
};

struct Box
{
    Coordinate* _pos;
    GLfloat _width;
    GLfloat _height;

    Box(Coordinate*, GLfloat, GLfloat);
};

struct Bullet
{
    Box*        _size;
    Coordinate* _direction;
    GLboolean   _isCollide;
    GLfloat     _xVelocity, _yVelocity;

    Bullet(Box*, Coordinate*, GLboolean, GLfloat, GLfloat);
};

#endif // UTILITY_H
