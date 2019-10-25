#include "Enemy.h"

Enemy::Enemy(GLfloat windowWidth, GLfloat windowHeight)
{
    srand(time(NULL));
    this->_tex       = new Texture();
    this->_xVelocity = 7.0f;
    this->_yVelocity = 7.0f;
    this->_effect    = new Sound();
}

Enemy::~Enemy()
{
    delete this->_tex;
    for (GLint i = 0; i < this->_enemy.size(); ++i) {
        delete this->_enemy[i];
    }
    delete this->_effect;
}

GLvoid Enemy::resetEnemy()
{
    for (GLint i = 0; i < this->_enemy.size(); ++i) {
        delete this->_enemy[i];
    }
    this->_enemy.clear();
}

Phase Enemy::moveEnemy(Box* player, Ground* ground)
{
    GLfloat playerLeft   = player->_pos->_xPos;
    GLfloat playerBottom = player->_pos->_yPos;
    GLfloat playerRight  = player->_pos->_xPos + player->_width;
    GLfloat playerTop    = player->_pos->_yPos + player->_height;

    GLfloat deltaTime = this->computeDeltaTime();

    for (GLint i = 0; i < this->_enemy.size(); ++i) {
        std::pair<GLfloat, GLfloat> fourPoints[4];
        fourPoints[0] = std::make_pair(this->_enemy[i]->_pos->_xPos, this->_enemy[i]->_pos->_yPos);
        fourPoints[1] = std::make_pair(this->_enemy[i]->_pos->_xPos + this->_enemy[i]->_width, this->_enemy[i]->_pos->_yPos);
        fourPoints[2] = std::make_pair(this->_enemy[i]->_pos->_xPos, this->_enemy[i]->_pos->_yPos + this->_enemy[i]->_height);
        fourPoints[3] = std::make_pair(this->_enemy[i]->_pos->_xPos + this->_enemy[i]->_width, this->_enemy[i]->_pos->_yPos + this->_enemy[i]->_height);

        for (GLint j = 0; j < 4; ++j) {
            if(fourPoints[j].first >= playerLeft &&
               fourPoints[j].first <= playerRight &&
               fourPoints[j].second >= playerBottom &&
               fourPoints[j].second <= playerTop) {
                return LOSE;
            }
        }

        GLfloat xDistance = playerLeft   - this->_enemy[i]->_pos->_xPos;
        GLfloat yDistance = playerBottom - this->_enemy[i]->_pos->_yPos;

        GLfloat xNew = pow(xDistance, 2.0f);
        GLfloat yNew = pow(yDistance, 2.0f);

        GLfloat magnitude = 1 / sqrt(xNew + yNew);
        Coordinate* normalize = new Coordinate(xDistance * magnitude, yDistance * magnitude);

        GLfloat xTemp = normalize->_xPos * deltaTime * this->_xVelocity;
        GLfloat yTemp = normalize->_yPos * deltaTime * this->_yVelocity;

        if (this->xCollision(this->_enemy[i], xTemp)) {
            this->_enemy[i]->_pos->_xPos += xTemp;
        }

        if(this->yCollision(this->_enemy[i], yTemp)) {
            this->_enemy[i]->_pos->_yPos += yTemp;
        }
    }
    return PLAY;
}

GLboolean Enemy::yCollision(Box* _size, GLfloat newPos)
{
    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < this->_enemy.size(); ++i) {
        GLfloat enemyLeft   = this->_enemy[i]->_pos->_xPos;
        GLfloat enemyBottom = this->_enemy[i]->_pos->_yPos;
        GLfloat enemyRight  = this->_enemy[i]->_pos->_xPos + this->_enemy[i]->_width;
        GLfloat enemyTop    = this->_enemy[i]->_pos->_yPos + this->_enemy[i]->_height;

        if (currLeft == enemyLeft && currBottom == enemyBottom &&
            currRight == enemyRight && currTop == enemyTop) {
            continue;
        }
        else {
            if (currBottom + newPos <= enemyTop &&
                currRight >= enemyLeft &&
                currLeft <= enemyRight &&
                currTop >= enemyTop) {
                _size->_pos->_yPos = enemyTop + 0.01f;
                return FALSE;
            }
            else if (currTop + newPos >= enemyBottom &&
                     currRight >= enemyLeft &&
                     currLeft <= enemyRight &&
                     currBottom <= enemyBottom) {
                _size->_pos->_yPos += enemyBottom - currTop - 0.01f;
                return FALSE;
            }
        }
    }
    return TRUE;
}

GLboolean Enemy::xCollision(Box* _size, GLfloat newPos)
{
    GLfloat currLeft   = _size->_pos->_xPos;
    GLfloat currBottom = _size->_pos->_yPos;
    GLfloat currRight  = _size->_pos->_xPos + _size->_width;
    GLfloat currTop    = _size->_pos->_yPos + _size->_height;

    for (int i = 0; i < this->_enemy.size(); ++i) {
        GLfloat enemyLeft   = this->_enemy[i]->_pos->_xPos;
        GLfloat enemyBottom = this->_enemy[i]->_pos->_yPos;
        GLfloat enemyRight  = this->_enemy[i]->_pos->_xPos + this->_enemy[i]->_width;
        GLfloat enemyTop    = this->_enemy[i]->_pos->_yPos + this->_enemy[i]->_height;

        if (currLeft == enemyLeft && currBottom == enemyBottom &&
            currRight == enemyRight && currTop == enemyTop) {
            continue;
        }
        else {
            if (currLeft + newPos <= enemyRight &&
                currBottom <= enemyTop &&
                currTop >= enemyBottom &&
                currRight >= enemyRight) {
                _size->_pos->_xPos = enemyRight + 0.01f;
                return FALSE;
            }
            else if (currRight + newPos >= enemyLeft &&
                     currBottom <= enemyTop &&
                     currTop >= enemyBottom &&
                     currLeft <= enemyLeft) {
                _size->_pos->_xPos += enemyLeft - currRight - 0.01f;
                return FALSE;
            }
        }
    }
    return TRUE;
}

std::vector<BOOL> Enemy::getCollide()
{
    return _areCollide;
}

GLvoid Enemy::setCollide(GLint j)
{
    this->_areCollide[j] = TRUE;
}

std::vector<Box*> Enemy::getEnemy()
{
    return this->_enemy;
}

GLvoid Enemy::spawnEnemy(GLfloat windowWidth, GLfloat windowHeight, std::vector<Box*> spawn)
{
    if (getTick() > 150.0f) {
        GLfloat newWidth  = windowWidth * 0.03f;
        GLfloat newHeight = windowHeight * 0.03f;
        GLint whichSpawn  = rand() % spawn.size();
        this->_enemy.push_back(new Box(new Coordinate(spawn[whichSpawn]->_pos->_xPos, spawn[whichSpawn]->_pos->_yPos), newWidth, newHeight));
        this->_areCollide.push_back(FALSE);
        this->resetTime();
    }
}

GLvoid Enemy::initializeEnemey(char* fileName)
{
    this->_tex->loadTexture(fileName);
}

GLvoid Enemy::drawEnemy()
{
    for (GLint i = 0; i < this->_enemy.size(); ++i) {
        this->_tex->bindTexture();
        this->_tex->drawTexture(this->_enemy[i]);
    }
}

GLvoid Enemy::scaleEnemy(GLfloat windowWidth, GLfloat windowHeight, GLfloat prevWidth, GLfloat prevHeight)
{
    for (GLint i = 0; i < this->_enemy.size(); ++i) {
        this->_enemy[i]->_pos->_xPos *= windowWidth  / prevWidth;
        this->_enemy[i]->_pos->_yPos *= windowHeight / prevHeight;
        this->_enemy[i]->_width      *= windowWidth  / prevWidth;
        this->_enemy[i]->_height     *= windowHeight / prevHeight;
        this->_xVelocity *= windowHeight / prevHeight;
        this->_yVelocity *= windowHeight / prevHeight;
    }
}

GLvoid Enemy::destroyEnemy(GLint& score)
{
    for (GLint i = 0; i < this->_areCollide.size(); ++i) {
        if(this->_areCollide[i] == TRUE) {
            delete this->_enemy[i];
            this->_enemy[i] = NULL;
            this->_enemy.erase(this->_enemy.begin() + i);
            this->_areCollide.erase(this->_areCollide.begin() + i);
            score += 5;
        }
    }
}
