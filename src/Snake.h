//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once

#ifndef QUETZALCOATL_SNAKE_H
#define QUETZALCOATL_SNAKE_H

#include "global.h"
#include <array>
#include <deque>
#include <utility>
#include <vector>

class Snake {
private:
    List m_body;
    std::array<std::array<bool, gridSize>, gridSize> m_positions{};
    Direction m_direction = Down;

public:
    Snake() = default;
    Snake(Snake const&) = default;
    Snake(List body, Direction direction);

    /**
     * Does nothing if <next> is not in the grid
     * @param next
     */
    void evolve(Coord next);
    /**
     * Does nothing if <next> is not in the grid
     * @param next
     */
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
    [[maybe_unused]] [[nodiscard]] List const& body() const;
    [[nodiscard]] Direction direction() const;
    /**
     * Does nothing if the given direction makes a U-turn.
     */
    void setDirection(Direction const&);
    /**
     * @return true if the given Coord is part of the body, flase otherwise.
     */
    bool contains(Coord const&);
    [[nodiscard]] size_t size() const;
    Coord const& operator[](size_t const& index) const;
};


#endif  // QUETZALCOATL_SNAKE_H
