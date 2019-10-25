#ifndef TEXTURE_H
#define TEXTURE_H

//Header Files
#include <Utility.h>

//C++ Libraries
#include <SOIL.h>
#include <GL/gl.h>
#include <stdio.h>

class Texture
{
    public:
        Texture();
        virtual ~Texture();

        //Bind, Load and Draw Texture
        GLvoid bindTexture();
        GLvoid loadTexture(char* );
        GLvoid drawTexture(Box* textureSize, GLboolean direction = TRUE);

    private:
        unsigned char* _image;
        GLint _width, _height;
        GLuint _tex;

        GLfloat _xMin, _xMax;
        GLfloat _yMin, _yMax;
};

#endif // TEXTURE_H
