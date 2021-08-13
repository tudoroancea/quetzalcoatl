//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Snake.h"

Coord Snake::tail(unsigned int const& offset) const {
    if (offset < m_body.size()) {
        return m_body[m_body.size() - 1 - offset];
    } else {
        return m_body.back();
    }
}
Coord Snake::head(unsigned int const& offset) const {
    if (offset < m_body.size()) {
        return m_body[offset];
    } else {
        return m_body.front();
    }
}
void Snake::evolve(Coord next) {
    m_body.push_front(next);
    m_body.pop_back();
}
void Snake::grow(Coord next) {
    m_body.push_front(next);
}
List Snake::body() const {
    return m_body;
}
Direction Snake::direction() const {
    return m_direction;
}
void Snake::setDirection(const Direction& newDirection) {
    this->m_direction = newDirection;
}
