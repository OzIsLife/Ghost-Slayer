#include "Ammunition.h"

Ammunition::Ammunition()
{
    this->_tex    = new Texture();
    this->_effect = new Sound();
}

Ammunition::~Ammunition()
{
    delete this->_tex;
    delete this->_effect;
}

GLvoid Ammunition::initializeAmmo(char* fileName)
{
    // gets image
    this->_tex->loadTexture(fileName);
}

GLvoid Ammunition::drawAmmo()
{
    this->_tex->bindTexture();
    for (GLint i = 0; i < _ammo.size(); ++i) {
        this->_tex->drawTexture(this->_ammo[i]->_size);
    }
}

GLvoid Ammunition::moveAmmo(Ground* ground)
{
    //reinitialzing pos
    for (GLint i = 0; i < _ammo.size(); ++i) {
        GLfloat deltaTime = this->computeDeltaTime();
        GLfloat xNewPos   = this->_ammo[i]->_direction->_xPos * this->_ammo[i]->_xVelocity * deltaTime;
        GLfloat yNewPos   = this->_ammo[i]->_direction->_yPos * this->_ammo[i]->_yVelocity * deltaTime;

        if(!this->xCollision(ground, this->_ammo[i]->_size, xNewPos)) {
             this->_ammo[i]->_isCollide = TRUE; // if collide is true then not collision write weird in rigid
        }
        else {
            this->_ammo[i]->_size->_pos->_xPos += xNewPos; // reinitialize the ammos position to the new position
        }

        if(!this->yCollision(ground, this->_ammo[i]->_size, yNewPos)) {
            this->_ammo[i]->_isCollide = TRUE;
            return;
        }
        else {
            this->_ammo[i]->_size->_pos->_yPos += yNewPos;
        }
    }
}

GLvoid Ammunition::addAmmo(Bullet* newBullet)
{
    this->_effect->playMusic("sounds/Bullet.mp3", FALSE);
    _ammo.push_back(newBullet);
}

GLvoid Ammunition::removeAmmo()
{
    //if collision delete bullet and reinitialze to NULL
    for (GLint i = 0; i < _ammo.size(); ++i) {
        if (_ammo[i]->_isCollide) {
            delete _ammo[i];
            _ammo[i] = NULL;
            _ammo.erase(_ammo.begin() + i);
        }
    }
}

GLvoid Ammunition::inRange(GLfloat windowWidth, GLfloat windowHeight)
{
    //Window range
    for (GLint i = 0; i < _ammo.size(); ++i) {
        if (this->_ammo[i]->_size->_pos->_xPos < 0.0f) {
            this->_ammo[i]->_size->_pos->_xPos = 0.0f;
            this->_ammo[i]->_isCollide = TRUE;
            return;
        }

        if (this->_ammo[i]->_size->_pos->_yPos < 0.0f) {
            this->_ammo[i]->_size->_pos->_yPos = 0.0f;
            this->_ammo[i]->_isCollide = TRUE;
            return;
        }

        if (this->_ammo[i]->_size->_pos->_xPos + this->_ammo[i]->_size->_width > windowWidth) {
            this->_ammo[i]->_size->_pos->_xPos = windowWidth - this->_ammo[i]->_size->_width;
            this->_ammo[i]->_isCollide = TRUE;
            return;
        }

        if (this->_ammo[i]->_size->_pos->_yPos + this->_ammo[i]->_size->_height > windowHeight) {
            this->_ammo[i]->_size->_pos->_yPos = windowHeight - this->_ammo[i]->_size->_height;
            this->_ammo[i]->_isCollide = TRUE;
            return;
        }
    }
}

//Scaling image and speed
GLvoid Ammunition::ammoScale(GLfloat windowWidth, GLfloat windowHeight, GLfloat prevWidth, GLfloat prevHeight)
{
    for (GLint i = 0; i < this->_ammo.size(); ++i) {
        this->_ammo[i]->_size->_pos->_xPos *= windowWidth  / prevWidth;
        this->_ammo[i]->_size->_pos->_yPos *= windowHeight / prevHeight;
        this->_ammo[i]->_size->_width      *= windowWidth  / prevWidth;
        this->_ammo[i]->_size->_height     *= windowHeight / prevHeight;
        this->_ammo[i]->_xVelocity *= windowHeight / prevHeight;
        this->_ammo[i]->_yVelocity *= windowHeight / prevHeight;;
    }
}

GLvoid Ammunition::hitEnemy(Enemy* enemy)
{
    std::vector<Box*> enemies = enemy->getEnemy();
    std::vector<BOOL> areCollide = enemy->getCollide();

        // reintializes current position of ammo
    for(GLint i = 0; i < this->_ammo.size(); ++i) {
        GLfloat currLeft   = this->_ammo[i]->_size->_pos->_xPos;
        GLfloat currRight  = this->_ammo[i]->_size->_pos->_xPos + this->_ammo[i]->_size->_width;
        GLfloat currBottom = this->_ammo[i]->_size->_pos->_yPos;
        GLfloat currTop    = this->_ammo[i]->_size->_pos->_yPos + this->_ammo[i]->_size->_height;

        //getting corners
        std::pair<GLfloat, GLfloat> currAmmo[4];
        currAmmo[0] = std::make_pair(currLeft, currBottom);
        currAmmo[1] = std::make_pair(currRight, currBottom);
        currAmmo[2] = std::make_pair(currLeft, currTop);
        currAmmo[3] = std::make_pair(currRight, currTop);

        // looping on enemy array
        for(GLint j = 0; j < enemies.size(); ++j) {
            GLfloat currLeft2   = enemies[j]->_pos->_xPos;
            GLfloat currRight2  = enemies[j]->_pos->_xPos + enemies[j]->_width;
            GLfloat currBottom2 = enemies[j]->_pos->_yPos;
            GLfloat currTop2    = enemies[j]->_pos->_yPos + enemies[j]->_height;

            //checks if bullet in enemy
            for (GLint k = 0; k < 4; ++k) {
                if (currAmmo[k].first >= currLeft2 &&
                    currAmmo[k].first <= currRight2 &&
                    currAmmo[k].second >= currBottom2 &&
                    currAmmo[k].second <= currTop2) {
                    enemy->setCollide(j);
                    this->_ammo[i]->_isCollide = TRUE;
                }
            }
        }
    }
}

GLboolean Ammunition::yCollision(Ground* ground, Box* _size, GLfloat newPos)
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

//resets ammo
GLvoid Ammunition::resetAmmo()
{
    for (GLint i = 0; i < this->_ammo.size(); ++i) {
        delete this->_ammo[i];
    }
    this->_ammo.clear();
}

GLboolean Ammunition::xCollision(Ground* ground, Box* _size, GLfloat newPos)
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
