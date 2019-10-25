#ifndef AMMUNITION_H
#define AMMUNITION_H

//Header Files
#include <Texture.h>
#include <Enemy.h>
#include <Utility.h>
#include <Ground.h>
#include <Sound.h>
#include <Timer.h>

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>
#include <utility>
#include <vector>

class Ammunition : public Timer
{
    public:
        Ammunition();
        virtual ~Ammunition();

        GLvoid initializeAmmo(char*);
        GLvoid drawAmmo();

        GLvoid moveAmmo(Ground*);
        GLvoid addAmmo(Bullet*);
        GLvoid removeAmmo();
        GLvoid inRange(GLfloat, GLfloat);
        GLvoid hitEnemy(Enemy*);
        GLvoid ammoScale(GLfloat, GLfloat, GLfloat, GLfloat);
        GLvoid resetAmmo();
        GLvoid stopEffect();

        //Collision
        GLboolean xCollision(Ground*, Box*, GLfloat);
        GLboolean yCollision(Ground*, Box*, GLfloat);

    private:
        Texture* _tex;
        std::vector<Bullet*> _ammo;
        Sound* _effect;
};

#endif // AMMUNITION_H
