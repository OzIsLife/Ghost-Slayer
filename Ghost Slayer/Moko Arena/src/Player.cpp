#include "Player.h"

Player::Player(GLfloat windowWidth, GLfloat windowHeight)
{
    srand(time(NULL));
    this->_player    = new Box(new Coordinate(10.0f, 10.0f), windowWidth * 0.3f, windowHeight * 0.3f);
    this->_gravity   = -9.8f;
    this->_xVelocity = 10.0f;
    this->_yVelocity = 12.0f;
    this->_animation = new Timer();
    this->_currIndex = 0;
    this->_direction = TRUE;
    this->_idle      = new Timer();

}

Player::~Player()
{
    delete _player;
    delete _animation;
    for (GLint i = 0; i < this->_tex.size(); ++i) {
        delete this->_tex[i];
    }
    this->_tex.clear();
}

GLvoid Player::resetPlayer()
{
    this->_player->_height = 0.0f;
    this->_player->_width = 0.0f;
    this->_player->_pos->_xPos = 0.0f;
    this->_player->_pos->_yPos = 0.0f;
    this->_currIndex = 0;
}

GLvoid Player::spawnPlayer(GLfloat windowWidth, GLfloat windowHeight, std::vector<Coordinate*> _playerSpawn)
{
    GLint spawn = rand() % _playerSpawn.size();
    this->_player->_height = windowHeight * 0.04f;
    this->_player->_width = windowWidth * 0.02f;
    this->_player->_pos->_xPos = _playerSpawn[spawn]->_xPos;
    this->_player->_pos->_yPos = _playerSpawn[spawn]->_yPos;
}

GLvoid Player::initializePlayer(char* fileName)
{
    std::string _fileNames[4] = {"images/Player/1.png", "images/Player/2.png", "images/Player/3.png", "images/Player/4.png"};

    for (GLint i = 0; i < 4; ++i) {
        this->_tex.push_back(new Texture());
        int stringSize = _fileNames[i].length();
        char* currChars = new char[stringSize + 1];
        strcpy(currChars, _fileNames[i].c_str());
        this->_tex[i]->loadTexture(currChars);
        delete currChars;
    }
}

Phase Player::idle()
{
    if (this->_player->_pos->_yPos == this->_prevY && this->_player->_pos->_xPos == this->_prevX) {
        if (this->_idle->getTick() > 300.0f) {
            return LOSE;
        }
    }

    return PLAY;

}

GLvoid Player::drawPlayer()
{
    this->_tex[this->_currIndex]->bindTexture();
    this->_tex[this->_currIndex]->drawTexture(this->_player, this->_direction);
    if (this->_animation->getTick() >= 700.0f) {
        ++this->_currIndex;
        this->_animation->resetTime();
        if (this->_currIndex >= this->_tex.size()) {
            this->_currIndex = 0;
        }
    }
}

GLvoid Player::getCurr()
{
    this->_animation->currTime();
    this->_idle->currTime();
}

GLvoid Player::getPrev()
{
    this->_animation->prevTime();
    this->_idle->prevTime();
}

GLvoid Player::moveDown(Ground* ground)
{
    GLfloat deltaTime = this->computeDeltaTime();
    GLfloat newPos = this->_gravity * deltaTime;
    this->_currIndex = 0;

    if (this->yCollision(ground, this->_player, newPos)) {
        this->_player->_pos->_yPos += newPos;
        this->_prevY = this->_player->_pos->_yPos;
    }
}

GLvoid Player::moveUp(Ground* ground)
{
    GLfloat deltaTime = this->computeDeltaTime();
    GLfloat newPos = this->_yVelocity * deltaTime;
    this->_currIndex = 0;

    if (this->yCollision(ground, this->_player, newPos)) {
        this->_player->_pos->_yPos += newPos;
        this->_prevY = this->_player->_pos->_yPos;
    }
}

GLvoid Player::moveLeft(Ground* ground)
{
    GLfloat deltaTime = this->computeDeltaTime();
    GLfloat newPos = -1 * this->_xVelocity * deltaTime;
    this->_currIndex = 0;
    this->_direction = TRUE;

    if (this->xCollision(ground, this->_player, newPos)) {
        this->_player->_pos->_xPos += newPos;
        this->_prevX = this->_player->_pos->_xPos;
    }
}

GLvoid Player::moveRight(Ground* ground)
{
    GLfloat deltaTime = this->computeDeltaTime();
    GLfloat newPos = this->_xVelocity * deltaTime;
    this->_currIndex = 0;
    this->_direction = FALSE;

    if (this->xCollision(ground, this->_player, newPos)) {
        this->_player->_pos->_xPos += newPos;
        this->_prevX = this->_player->_pos->_xPos;
    }
}

GLvoid Player::playerScale(GLfloat windowWidth, GLfloat windowHeight, GLfloat prevWidth, GLfloat prevHeight)
{
    this->_player->_pos->_xPos *= windowWidth  / prevWidth;
    this->_player->_pos->_yPos *= windowHeight / prevHeight;
    this->_player->_width      *= windowWidth  / prevWidth;
    this->_player->_height     *= windowHeight / prevHeight;
    this->_xVelocity *= windowHeight / prevHeight;
    this->_yVelocity *= windowHeight / prevHeight;;
}

Bullet* Player::playerShoot(Coordinate* mouse)
{
    GLfloat delayTrigger = 120.0f; //In weapon class
    if (this->getTick() > delayTrigger) {
        GLfloat xDistance = mouse->_xPos - this->_player->_pos->_xPos;
        GLfloat yDistance = mouse->_yPos - this->_player->_pos->_yPos;

        if (xDistance != 0.0f || yDistance != 0.0f) {
            GLfloat xNew = pow(xDistance, 2.0f);
            GLfloat yNew = pow(yDistance, 2.0f);
            GLfloat magnitude = 1 / sqrt(xNew + yNew);
            Coordinate* normalize = new Coordinate(xDistance * magnitude,
                                                   yDistance * magnitude);

            GLfloat xTemp         = this->_player->_pos->_xPos + (this->_player->_width / 2);
            GLfloat yTemp         = this->_player->_pos->_yPos + (this->_player->_height / 2);
            GLfloat widthTemp     = this->_player->_width  * 0.1f;
            GLfloat heightTemp    = this->_player->_height * 0.1f;
            Coordinate* coordTemp = new Coordinate(xTemp, yTemp);
            Box* sizeTemp         = new Box(coordTemp, widthTemp, heightTemp);
            Bullet* bulletTemp    = new Bullet(sizeTemp, normalize, FALSE, 15.0f, 15.0f);
            this->resetTime();

            return bulletTemp;
        }
    }
    return NULL;
}

GLvoid Player::inRange(GLfloat windowWith, GLfloat windowHeight)
{
    if (this->_player->_pos->_xPos < 0.0f) {
        this->_player->_pos->_xPos = 0.0f;
    }

    if (this->_player->_pos->_yPos < 0.0f) {
        this->_player->_pos->_yPos = 0.0f;
    }

    if (this->_player->_pos->_xPos + this->_player->_width > windowWith) {
        this->_player->_pos->_xPos = windowWith - this->_player->_width;
    }

    if (this->_player->_pos->_yPos + this->_player->_height > windowHeight) {
        this->_player->_pos->_yPos = windowHeight - this->_player->_height;
    }
}

Box* Player::getPlayerBox()
{
    return _player;
}

GLboolean Player::yCollision(Ground* ground, Box* _size, GLfloat newPos)
{
    std::vector<Box*> grounds = ground->getGround();

    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < grounds.size(); ++i) {
        GLfloat groundLeft   = grounds[i]->_pos->_xPos;
        GLfloat groundBottom = grounds[i]->_pos->_yPos;
        GLfloat groundRight  = grounds[i]->_pos->_xPos + grounds[i]->_width;
        GLfloat groundTop    = grounds[i]->_pos->_yPos + grounds[i]->_height;

        if (currBottom + newPos <= groundTop &&
            currRight >= groundLeft &&
            currLeft <= groundRight &&
            currTop >= groundTop) {
            _size->_pos->_yPos = groundTop + 0.01f;
            return FALSE;
        }
        else if (currTop + newPos >= groundBottom &&
                 currRight >= groundLeft &&
                 currLeft <= groundRight &&
                 currBottom <= groundBottom) {
            _size->_pos->_yPos += groundBottom - currTop - 0.01f;
            return FALSE;
        }
    }
    return TRUE;
}

GLboolean Player::xCollision(Ground* ground, Box* _size, GLfloat newPos)
{
    std::vector<Box*> grounds = ground->getGround();

    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < grounds.size(); ++i) {
        GLfloat groundLeft   = grounds[i]->_pos->_xPos;
        GLfloat groundBottom = grounds[i]->_pos->_yPos;
        GLfloat groundRight  = grounds[i]->_pos->_xPos + grounds[i]->_width;
        GLfloat groundTop    = grounds[i]->_pos->_yPos + grounds[i]->_height;

        if (currLeft + newPos <= groundRight &&
            currBottom <= groundTop &&
            currTop >= groundBottom &&
            currRight >= groundRight) {
            _size->_pos->_xPos = groundRight + 0.01f;
            return FALSE;
        }
        else if (currRight + newPos >= groundLeft &&
                 currBottom <= groundTop &&
                 currTop >= groundBottom &&
                 currLeft <= groundLeft) {
            _size->_pos->_xPos += groundLeft - currRight - 0.01f;
            return FALSE;
        }
    }
    return TRUE;
}
