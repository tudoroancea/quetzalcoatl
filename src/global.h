//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once
#ifndef QUETZALCOATL_CONST_H
#define QUETZALCOATL_CONST_H

#include <deque>
#include <QColor>
#include <array>

#define NUM_TYPE unsigned int

constexpr NUM_TYPE gridSize = 15;
constexpr unsigned int snakeBaseSize = 4;
constexpr std::array<int,7> simulationTabSpeed =  {210, 180, 150, 120, 90, 60, 30};
const QColor darkGreen = QColor::fromRgb(162, 209, 72);
const QColor lightGreen = QColor::fromRgb(169,215,80);
const QColor darkBlue = QColor::fromRgb(71,116,233);
const QColor darkRed = QColor::fromRgb(231,71,29);

typedef std::pair<NUM_TYPE, NUM_TYPE> Coord;
bool operator==(Coord const& lhs, Coord const& rhs);
bool operator!=(Coord const& lhs, Coord const& rhs);
typedef std::deque<Coord> List;
enum Direction { Up,
                 Down,
                 Left,
                 Right };


#endif  // QUETZALCOATL_CONST_H