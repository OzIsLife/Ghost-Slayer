#include "Texture.h"

Texture::Texture()
{
    _xMin = _yMin = 0.0f;
    _xMax = _yMax = 1.0f;
}

Texture::~Texture()
{
    _image = NULL;
}

GLvoid Texture::bindTexture()
{
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, _tex);
}

GLvoid Texture::loadTexture(char* fileName)
{
    glGenTextures(1, &_tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, _tex);
    _image = SOIL_load_image(fileName, &_width, &_height, 0, SOIL_LOAD_RGBA);

    if(!_image) printf("Fail to find image: %s\n", fileName);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image);
    SOIL_free_image_data(_image);

    glEnable(GL_TEXTURE_2D);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLvoid Texture::drawTexture(Box* textureSize, GLboolean direction)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    if (direction) {
         //Top-Left Corner to Bottom-Left Corner
        glTexCoord2f(this->_xMin, this->_yMax);
        glVertex2f(textureSize->_pos->_xPos, textureSize->_pos->_yPos);

        //Top-Right Corner to Bottom-Right Corner
        glTexCoord2f(this->_xMax, this->_yMax);
        glVertex2f(textureSize->_pos->_xPos + textureSize->_width, textureSize->_pos->_yPos);

        //Bottom-Right Corner to Top-Right Corner
        glTexCoord2f(this->_xMax, this->_yMin);
        glVertex2f(textureSize->_pos->_xPos + textureSize->_width, textureSize->_pos->_yPos + textureSize->_height);

        //Bottom-Left Corner to Top-Left Corner
        glTexCoord2f(this->_xMin, this->_yMin);
        glVertex2f(textureSize->_pos->_xPos, textureSize->_pos->_yPos + textureSize->_height);
    }
    else {
        //Top-Left Corner to Bottom-Right Corner
        glTexCoord2f(this->_xMin, this->_yMax);
        glVertex2f(textureSize->_pos->_xPos + textureSize->_width, textureSize->_pos->_yPos);

        //Top-Right Corner to Bottom-Left Corner
        glTexCoord2f(this->_xMax, this->_yMax);
        glVertex2f(textureSize->_pos->_xPos, textureSize->_pos->_yPos);

        //Bottom-Right Corner to Top-Left Corner
        glTexCoord2f(this->_xMax, this->_yMin);
        glVertex2f(textureSize->_pos->_xPos, textureSize->_pos->_yPos + textureSize->_height);

        //Bottom-Left Corner to Top-Right Corner
        glTexCoord2f(this->_xMin, this->_yMin);
        glVertex2f(textureSize->_pos->_xPos + textureSize->_width, textureSize->_pos->_yPos + textureSize->_height);
    }
    glEnd();
    glPopMatrix();
}
