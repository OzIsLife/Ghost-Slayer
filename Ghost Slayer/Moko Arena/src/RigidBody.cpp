#include "RigidBody.h"

RigidBody::RigidBody()
{
    _tex = new Texture();
}

RigidBody::~RigidBody()
{
    delete _tex;
}

GLboolean RigidBody::xCollision(Ground* ground, Box*& _size, GLfloat newPos)
{
    std::vector<Box*> grounds = ground->getGround();

    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < grounds.size(); ++i) {
        GLfloat groundLeft   = grounds[i]->_pos->_xPos;
        GLfloat groundBottom = grounds[i]->_pos->_yPos;
        GLfloat groundRight  = grounds[i]->_pos->_xPos + grounds[i]->_width;
        GLfloat groundTop    = grounds[i]->_pos->_yPos + grounds[i]->_height;

        if (currLeft + newPos <= groundRight &&
            currBottom <= groundTop &&
            currTop >= groundBottom &&
            currRight >= groundRight) {
            _size->_pos->_xPos = groundRight + 0.01f;
            return FALSE;
        }
        else if (currRight + newPos >= groundLeft &&
                 currBottom <= groundTop &&
                 currTop >= groundBottom &&
                 currLeft <= groundLeft) {
            _size->_pos->_xPos += groundLeft - currRight - 0.01f;
            return FALSE;
        }
    }
    return TRUE;
}

GLboolean RigidBody::yCollision(Ground* ground, Box*& _size, GLfloat newPos)
{
    std::vector<Box*> grounds = ground->getGround();

    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < grounds.size(); ++i) {
        GLfloat groundLeft   = grounds[i]->_pos->_xPos;
        GLfloat groundBottom = grounds[i]->_pos->_yPos;
        GLfloat groundRight  = grounds[i]->_pos->_xPos + grounds[i]->_width;
        GLfloat groundTop    = grounds[i]->_pos->_yPos + grounds[i]->_height;

        if (currBottom + newPos <= groundTop &&
            currRight >= groundLeft &&
            currLeft <= groundRight &&
            currTop >= groundTop) {
            _size->_pos->_yPos = groundTop + 0.01f;
            return FALSE;
        }
        else if (currTop + newPos >= groundBottom &&
                 currRight >= groundLeft &&
                 currLeft <= groundRight &&
                 currBottom <= groundBottom) {
            _size->_pos->_yPos += groundBottom - currTop - 0.01f;
            return FALSE;
        }
    }
    return TRUE;
}
