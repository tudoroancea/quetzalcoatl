//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Snake.h"

#include <utility>


Snake::Snake(List body, Direction direction) : m_body(std::move(body)), m_direction(direction) {
    for (auto const& el : m_body) {
        m_positions[el.first][el.second] = true;
    }
}

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
    if (next.first <= 14 && next.second <= 14) {
        m_positions[next.first][next.second] = true;
        m_body.push_front(next);
        m_positions[m_body.back().first][m_body.back().second] = false;
        m_body.pop_back();
    }
}
void Snake::grow(Coord next) {
    if (next.first <= 14 && next.second <= 14) {
        m_positions[next.first][next.second] = true;
        m_body.push_front(next);
    }
}
[[maybe_unused]] List const& Snake::body() const {
    return m_body;
}
Direction Snake::direction() const {
    return m_direction;
}
void Snake::setDirection(const Direction& newDirection) {
    switch (newDirection) {
        case Up:
            if (m_direction != Down && this->head(1) != this->head() + UpCoord) {
                m_direction = Up;
            }
            break;
        case Down:
            if (m_direction != Up && this->head(1) != this->head() + DownCoord) {
                m_direction = Down;
            }
            break;
        case Left:
            if (m_direction != Right && this->head(1) != this->head() + LeftCoord) {
                m_direction = Left;
            }
            break;
        case Right:
            if (m_direction != Left && this->head(1) != this->head() + RightCoord) {
                m_direction = Right;
            }
            break;
    }
}
bool Snake::contains(const Coord& candidate) {
    return m_positions[candidate.first][candidate.second];
}
size_t Snake::size() const {
    return m_body.size();
}
Coord const& Snake::operator[](size_t const& index) const {
    return m_body[index];
}
