//
// Created by berth on 12/08/2021.
//

#ifndef QUETZALCOATL_GAME_H
#define QUETZALCOATL_GAME_H

#include "Snake.h"

#include <deque>


class Game {
private:
    Snake snake;
    Coord apple;
    unsigned score = 0;

public:
    Game();
    Coord regenApple();
    unsigned getScore();
    Coord getApple();
    void setScore();
    void setApple();
    void update();
    Snake& getSnake();

};
#endif //QUETZALCOATL_GAME_H
