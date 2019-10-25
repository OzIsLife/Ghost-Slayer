#include "Sound.h"

Sound::Sound()
{
    this->_engine = createIrrKlangDevice();
}

Sound::~Sound()
{
    delete this->_engine;
}

void Sound::playMusic(char* fileName, GLboolean loop)
{
    this->_engine->play2D(fileName, loop);
}

void Sound::stopMusic()
{
    this->_engine->stopAllSounds();
}
