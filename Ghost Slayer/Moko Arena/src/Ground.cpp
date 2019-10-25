#include "Ground.h"

Ground::Ground()
{

}

Ground::~Ground()
{
    for (int i = 0; i < this->_grounds.size(); ++i) {
        delete this->_grounds[i];
    }

    for (int i = 0; i < this->_tex.size(); ++i) {
        delete this->_tex[i];
    }

    this->_grounds.clear();
    this->_tex.clear();
}

GLvoid Ground::drawGround()
{
    for (GLint i = 0; i < this->_grounds.size(); ++i) {
        this->_tex[i]->bindTexture();
        this->_tex[i]->drawTexture(this->_grounds[i]);
    }
}

GLvoid Ground::groundScale(GLfloat windowWidth, GLfloat windowHeight, GLfloat prevWidth, GLfloat prevHeight)
{
    for (GLint i = 0; i < this->_grounds.size(); ++i) {
        this->_grounds[i]->_pos->_xPos *= windowWidth  / prevWidth;
        this->_grounds[i]->_pos->_yPos *= windowHeight / prevHeight;
        this->_grounds[i]->_width      *= windowWidth  / prevWidth;
        this->_grounds[i]->_height     *= windowHeight / prevHeight;
    }
}

GLvoid Ground::loadGround(GLfloat windowWidth, GLfloat windowHeight, std::vector<Box*>& enemySpawn, std::vector<Coordinate*>& playerSpawn)
{
    std::ifstream chosenMap("maps/Map.txt");
    std::string currLine_;

    if (chosenMap.is_open()) {
        std::getline(chosenMap, currLine_);
        GLfloat width = atoi(currLine_.c_str());

        std::getline(chosenMap, currLine_);
        GLfloat height = atoi(currLine_.c_str());

        for (int i = 0; std::getline(chosenMap, currLine_); ++i) {
            for (int j = 0; j < currLine_.size(); ++j) {
                GLfloat x = windowWidth - (windowWidth - ((windowWidth/ width) * j));
                GLfloat y = windowHeight - ((windowHeight / height) * (i + 1));
                Coordinate* posTemp = new Coordinate(x, y);
                Box*        boxTemp = new Box(posTemp, windowWidth / width, windowHeight / height);
                switch(currLine_[j]) {
                    case '1': {
                        this->_grounds.push_back(boxTemp);
                        this->_tex.push_back(new Texture());
                        this->_tex[this->_tex.size() - 1]->loadTexture("images/Tiles/1.png");
                    } break;
                    case '2': {
                        this->_grounds.push_back(boxTemp);
                        this->_tex.push_back(new Texture());
                        this->_tex[this->_tex.size() - 1]->loadTexture("images/Tiles/2.png");
                    } break;
                    case '3': {
                        this->_grounds.push_back(boxTemp);
                        this->_tex.push_back(new Texture());
                        this->_tex[this->_tex.size() - 1]->loadTexture("images/Tiles/3.png");
                    } break;
                    case '5': {
                        this->_grounds.push_back(boxTemp);
                        this->_tex.push_back(new Texture());
                        this->_tex[this->_tex.size() - 1]->loadTexture("images/Tiles/5.png");
                    } break;
                    case '9': {
                        this->_grounds.push_back(boxTemp);
                        this->_tex.push_back(new Texture());
                        this->_tex[this->_tex.size() - 1]->loadTexture("images/Tiles/9.png");
                    } break;
                    case 'E': {
                        enemySpawn.push_back(boxTemp);
                    } break;
                    case 'P': {
                        playerSpawn.push_back(posTemp);
                    }

                }
            }
        }
    }
}

std::vector<Box*> Ground::getGround()
{
    return this->_grounds;
}
