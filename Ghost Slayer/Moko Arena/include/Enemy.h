#ifndef ENEMY_H
#define ENEMY_H

//Header Files
#include <Player.h>
#include <Texture.h>
#include <Timer.h>
#include <Sound.h>
#include <Utility.h>

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include <utility>
#include <time.h>

class Enemy : public Timer
{
    public:
        Enemy(GLfloat, GLfloat);
        virtual ~Enemy();

        Phase moveEnemy(Box*, Ground*);
        GLvoid initializeEnemey(char*);
        GLvoid spawnEnemy(GLfloat, GLfloat, std::vector<Box*>);
        GLvoid drawEnemy();
        GLvoid scaleEnemy(GLfloat, GLfloat, GLfloat, GLfloat);
        GLvoid destroyEnemy(GLint&);
        GLvoid resetEnemy();

        GLboolean xCollision(Box*, GLfloat);
        GLboolean yCollision(Box*, GLfloat);

        std::vector<Box*> getEnemy();
        std::vector<BOOL> getCollide();
        GLvoid setCollide(GLint);

    private:
        Texture* _tex;
        std::vector<Box*> _enemy;
        std::vector<BOOL> _areCollide;
        GLfloat _xVelocity, _yVelocity;
        Sound* _effect;
};

#endif // ENEMY_H
