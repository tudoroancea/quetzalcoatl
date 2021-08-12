//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Snake.h"

Coord Snake::tail() {
    return body.back();
}
Coord Snake::head() {
    return body.front();
}
void Snake::evolve(Coord next) {
    body.push_front(next);
    body.pop_back();
}
void Snake::grow(Coord next) {
    body.push_front(next);
}
List Snake::getBody() const {
    return body;
}
Direction Snake::getDirection() const {
    return direction;
}
void Snake::setDirection(const Direction& newDirection) {
    this->direction = newDirection;
}
