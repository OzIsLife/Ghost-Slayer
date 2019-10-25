#include "Utility.h"

Coordinate::Coordinate(GLfloat xNew, GLfloat yNew) {
    this->_xPos = xNew;
    this->_yPos = yNew;
}

Box::Box(Coordinate* newPos, GLfloat newWidth, GLfloat newHeight) {
    this->_pos    = newPos;
    this->_width  = newWidth;
    this->_height = newHeight;
}

Bullet::Bullet(Box* newSize,
               Coordinate* newDirection,
               GLboolean isCollide,
               GLfloat xVelocity, GLfloat yVelociry)
{
    _size      = newSize;
    _direction = newDirection;
    _isCollide = isCollide;
    _xVelocity = xVelocity;
    _yVelocity = yVelociry;
}
