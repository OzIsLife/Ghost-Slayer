#ifndef GROUND_H
#define GROUND_H

//Header Files
#include <Texture.h>
#include <Utility.h>

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class Ground
{
    public:
        Ground();
        virtual ~Ground();

        //Draw
        GLvoid drawGround();
        GLvoid groundScale(GLfloat, GLfloat, GLfloat, GLfloat);
        GLvoid loadGround(GLfloat, GLfloat, std::vector<Box*>&, std::vector<Coordinate*>&);
        std::vector<Box*> getGround();

    private:
        std::vector<Texture*> _tex;
        std::vector<Box*>     _grounds;
};

#endif // GROUND_H
