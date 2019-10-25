#include "Menu.h"

Menu::Menu(GLfloat windowWidth, GLfloat windowHeight, GLint numSize, Box* ratioSize, Coordinate* gapSize, Phase* newPhase)
{
    this->_numSize = numSize;
    this->_tex   = new Texture*[this->_numSize];
    this->_sizes = new Box*[this->_numSize];
    this->_scopePhase = new Phase[this->_numSize];

    for (GLint i = 0; i < this->_numSize; ++i) {
        this->_scopePhase[i] = newPhase[i];
        this->_tex[i]        = new Texture();
        GLfloat x            = (windowWidth  * ratioSize->_pos->_xPos) - (windowWidth  * gapSize->_xPos);
        GLfloat y            = (windowHeight * ratioSize->_pos->_xPos) - (windowHeight * gapSize->_yPos);
        GLfloat width        = (windowWidth  * ratioSize->_width);
        GLfloat height       = (windowHeight * ratioSize->_height);
        this->_sizes[i]      = new Box(new Coordinate(x, y), width, height);

        if (gapSize->_xPos > 0.0f) {
            gapSize->_xPos += 0.09f;
        }

        if (gapSize->_yPos > 0.0f) {
            gapSize->_yPos += 0.09f;
        }
    }
}

Menu::~Menu()
{
    for (GLint i = 0; i < this->_numSize; ++i) {
        delete this->_tex[i];
        delete this->_sizes[i];
        delete this->_scopePhase;
    }

    delete [] this->_tex;
    delete [] this->_sizes;
    delete [] this->_scopePhase;
}

GLvoid Menu::initializeMain(std::string* pathNames)
{
    for (GLint i = 0; i < this->_numSize; ++i) {
        int stringSize = pathNames[i].length();
        char* currChars = new char[stringSize + 1];
        strcpy(currChars, pathNames[i].c_str());
        this->_tex[i]->loadTexture(currChars);
        delete currChars;
    }
}

GLvoid Menu::drawMain()
{
    for (GLint i = this->_numSize - 1; i >= 0; --i) {
        this->_tex[i]->bindTexture();
        this->_tex[i]->drawTexture(this->_sizes[i]);
    }
}

GLvoid Menu::scaleMain(GLfloat windowWidth, GLfloat windowHeight, GLfloat prevWidth, GLfloat prevHeight)
{
    for (GLint i = 0; i < this->_numSize; ++i) {
        this->_sizes[i]->_pos->_xPos *= windowWidth  / prevWidth;
        this->_sizes[i]->_pos->_yPos *= windowHeight / prevHeight;
        this->_sizes[i]->_width      *= windowWidth  / prevWidth;
        this->_sizes[i]->_height     *= windowHeight / prevHeight;
    }
}

Phase Menu::whichPhase(Coordinate* mouse)
{
    for (GLint i = 0; i < this->_numSize; ++i) {
        if(mouse->_xPos >= this->_sizes[i]->_pos->_xPos &&
           mouse->_xPos <= this->_sizes[i]->_pos->_xPos + this->_sizes[i]->_width &&
           mouse->_yPos >= this->_sizes[i]->_pos->_yPos &&
           mouse->_yPos <= this->_sizes[i]->_pos->_yPos + this->_sizes[i]->_height) {
        return this->_scopePhase[i];
        }
    }

    return NONE;
}
