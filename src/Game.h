//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#ifndef QUETZALCOATL_GAME_H
#define QUETZALCOATL_GAME_H

#include "Snake.h"
#include "global.h"

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
    void error();
    Coord avancer();

};
#endif //QUETZALCOATL_GAME_H
