//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#ifndef QUETZALCOATL_GAME_H
#define QUETZALCOATL_GAME_H

#include "Snake.h"
#include "global.h"

#include <QObject>
#include <deque>


class Game : public QObject {
    Q_OBJECT
private:
    Snake snake;
    Coord apple;
    unsigned score = 0;
    bool isFinished = false;

    //    Utility functions ======================

    Coord regenApple();
    [[nodiscard]] Coord avancer() const;

public:
    Game();

    //    Getters & Setters ========================

    [[nodiscard]] unsigned getScore() const;
    void setScore(unsigned int const& newScore);
    [[nodiscard]] Coord getApple() const;
    void setApple(Coord const& newApple);
    Snake& getSnake();
    bool getIsFinished();
    //    Evolution =============================

    void update();

signals:
    void error();
};
#endif  // QUETZALCOATL_GAME_H
