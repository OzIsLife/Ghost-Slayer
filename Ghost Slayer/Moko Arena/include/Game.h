#ifndef GAME_H
#define GAME_H

//Header Files
#include <Player.h>
#include <Enemy.h>
#include <Ammunition.h>
#include <Ground.h>
#include <Sound.h>
#include <Light.h>
#include <Menu.h>
#include <Utility.h>

//C++ libraries
#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>

class Game : Light
{
    public:
        Game(GLfloat, GLfloat);
        virtual ~Game();

        //Game
        GLint  initializeGame();
        GLint  drawGame();
        GLvoid resizeGame(GLsizei, GLsizei);

        //Input
        GLvoid buttonPressed(WPARAM);               //Turn a key to true
        GLvoid buttonReleased(WPARAM);              //Turn a key to false
        GLvoid mousePressed();
        GLboolean isButtonTrue(WPARAM);             //Check if current key is true

        //Time
        GLvoid settingCurrTime();
        GLvoid settingPrevTime();

        //Update
        GLvoid updatingMouse(GLfloat, GLfloat);
        GLvoid updatingGame();

        //Other
        GLvoid destroyItem();
        GLboolean currPrase();

        GLvoid stopEffect();

    private:
        Box*        _screen;
        Player*     _player;
        Enemy*      _enemies;
        Ammunition* _activeAmmo;
        Ground*     _ground;
        Coordinate* _mouse;
        Phase       _currPhase;
        Menu*       _mainScene;
        Sound*      _sound;


        std::vector<Box*> _enemySpawn;
        std::vector<Coordinate*> _playerSpawn;
        GLboolean   _areKeys[256];
        GLboolean   _isExitFromPlay;
        Texture*    _playBackground;
        Texture*    _play;
        Texture*    _howToPlayBackground;
        Texture*    _lose;
        GLint     _score;
};

#endif // GAME_H
