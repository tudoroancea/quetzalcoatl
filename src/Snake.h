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
    List m_body;
    Direction m_direction = Down;

public:
    Snake() = default;
    Snake(Snake const&) = default;
    Snake(List body, Direction direction) : m_body(std::move(body)), m_direction(direction) {}

    void evolve(Coord next);
    void grow(Coord next);

    //    Getters & Setters ========================
    /**
     * Return the body cell which is the <offset>th neighbor of the tail.
     * @param offset
     * @return the tail itself if the <offset> is ill conditioned.
     */
    [[nodiscard]] Coord tail(unsigned int const& offset = 0) const;
    /**
     * Return the body cell which is the <offset>th neighbor of the head.
     * @param offset
     * @return the head itself if the <offset> is ill conditioned.
     */
    [[nodiscard]] Coord head(unsigned int const& offset = 0) const;
    [[nodiscard]] List body() const;
    [[nodiscard]] Direction direction() const;
    void setDirection(Direction const& newDirection);
};


#endif  // QUETZALCOATL_SNAKE_H
