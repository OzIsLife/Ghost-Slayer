#ifndef PLAYER_H
#define PLAYER_H

//Header Files
#include <Ground.h>
#include <Texture.h>
#include <Timer.h>
#include <Utility.h>

//C++ Libraries
#include <windows.h>
#include <GL/gl.h>
#include <utility>
#include <time.h>
#include <cmath>
#include <vector>
#include <string>

class Player : public Timer
{
    public:
        Player(GLfloat, GLfloat);
        virtual ~Player();

        //Texture
        GLvoid initializePlayer(char*);
        GLvoid drawPlayer();

        //Move
        GLvoid moveDown(Ground*);
        GLvoid moveUp(Ground*);
        GLvoid moveLeft(Ground*);
        GLvoid moveRight(Ground*);

        //Collision
        GLboolean xCollision(Ground*, Box*, GLfloat);
        GLboolean yCollision(Ground*, Box*, GLfloat);

        //Other
        GLvoid  playerScale(GLfloat, GLfloat, GLfloat, GLfloat);
        Bullet* playerShoot(Coordinate*);
        GLvoid  inRange(GLfloat, GLfloat);
        GLvoid  resetPlayer();
        GLvoid  spawnPlayer(GLfloat, GLfloat, std::vector<Coordinate*>);
        Box*    getPlayerBox();
        GLvoid  getCurr();
        GLvoid  getPrev();
        Phase  idle();

    private:
        Box*     _player;
        Box*     _collision;
        std::vector<Texture*> _tex;
        Timer* _animation;
        Timer* _idle;
        GLfloat  _gravity;
        GLint _currIndex;
        GLfloat  _xVelocity, _yVelocity;
        GLfloat _prevX, _prevY;
        GLboolean _direction;


};

#endif // PLAYER_H
