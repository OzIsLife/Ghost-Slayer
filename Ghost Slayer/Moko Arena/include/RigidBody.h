#ifndef RIGIDBODY_H
#define RIGIDBODY_H

//Header Files
#include <Texture.h>
#include <Ground.h>
#include <Utility.h>

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>

class RigidBody
{
    public:
        RigidBody();
        virtual ~RigidBody();

        //Collision
        GLboolean xCollision(Ground*, Box*&, GLfloat);
        GLboolean yCollision(Ground*, Box*&, GLfloat);

    protected:
        Texture* _tex;
};

#endif // RIGIDBODY_H
