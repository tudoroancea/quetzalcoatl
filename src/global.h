//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#ifndef CONST_H
#define CONST_H

#include <deque>

#define NUM_TYPE unsigned int

NUM_TYPE gridSize = 15;
NUM_TYPE snakeBaseSize = 4;
int simulationBaseSpeed = 100;


typedef std::pair<NUM_TYPE, NUM_TYPE> Coord;
typedef std::deque<Coord> List;
enum Direction {Up, Down, Left, Right};


#endif