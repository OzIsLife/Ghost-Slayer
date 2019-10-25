#ifndef SOUND_H
#define SOUND_H

//Header Files
#include <SNDS/irrKlang.h>


//C++ libraries
#include <windows.h>
#include <GL/gl.h>

#pragma comment(lib,"irrKlang.lib")
using namespace irrklang;

class Sound
{
    public:
        Sound();
        virtual ~Sound();
        void playMusic(char*, GLboolean);
        void stopMusic();

    protected:

    private:
        ISoundEngine* _engine;
};

#endif // SOUND_H
