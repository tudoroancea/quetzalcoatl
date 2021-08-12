//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once

#ifndef QUETZALCOATL_SNAKE_H
#define QUETZALCOATL_SNAKE_H

#include "global.h"
#include <deque>
#include <utility>

class Snake {
private:
    List body;
    Direction direction = Down;

public:
    Snake() = default;
    Snake(Snake const&) = default;
    explicit Snake(List body_) : body(std::move(body_)) {}

    void evolve(Coord next);
    void grow(Coord next);

    //    Getters & Setters ========================

    Coord tail();
    Coord head();
    [[nodiscard]] List getBody() const;
    [[nodiscard]] Direction getDirection() const;
    void setDirection(Direction const& newDirection);
};


#endif  // QUETZALCOATL_SNAKE_H
