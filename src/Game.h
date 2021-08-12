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


class Game {
private:
    Snake m_snake;
    Coord m_apple;
    unsigned int m_score = 0;
    bool m_isFinished = false;
    bool m_hasBegun = false;

    //    Utility functions ======================

    [[nodiscard]] Coord regenerateApple();
    [[nodiscard]] Coord nextStep() const;

public:
    Game();
    Game(Game const&) = default;

    //    Getters & Setters ========================

    [[nodiscard]] unsigned score() const;
    [[nodiscard]] Coord apple() const;
    [[nodiscard]] Snake& snake();
    [[nodiscard]] bool isFinished() const;
    [[nodiscard]] bool hasBegun() const;

    //    Evolution =============================

    void begin();
    void update();
};
#endif  // QUETZALCOATL_GAME_H
