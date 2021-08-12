//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once
#ifndef QUETZALCOATL_CONST_H
#define QUETZALCOATL_CONST_H

#include <deque>

#define NUM_TYPE unsigned int

NUM_TYPE gridSize = 15;
NUM_TYPE snakeBaseSize = 4;
int simulationBaseSpeed = 100;


typedef std::pair<NUM_TYPE, NUM_TYPE> Coord;
bool operator==(Coord const& lhs, Coord const& rhs) { return lhs.first == rhs.first && lhs.second == rhs.second; }
bool operator!=(Coord const& lhs, Coord const& rhs) { return !(lhs == rhs); }
typedef std::deque<Coord> List;
enum Direction { Up,
                 Down,
                 Left,
                 Right };


#endif  // QUETZALCOATL_CONST_H