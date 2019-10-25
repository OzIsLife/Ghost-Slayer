#ifndef MENU_H
#define MENU_H

//Header Files
#include <Texture.h>
#include <Utility.h>

//C++ Libraries
#include <windows.h>
#include <bits/stdc++.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

class Menu
{
    public:
        Menu(GLfloat, GLfloat, GLint, Box*, Coordinate*, Phase*);
        ~Menu();
        GLvoid initializeMain(std::string*);
        GLvoid drawMain();
        Phase  whichPhase(Coordinate*);
        GLvoid scaleMain(GLfloat, GLfloat, GLfloat, GLfloat);

    private:
        Texture** _tex;
        Box**     _sizes;
        GLint     _numSize;
        Phase*    _scopePhase;
};

#endif // MENU_H
