//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once
#ifndef QUETZALCOATL_CONST_H
#define QUETZALCOATL_CONST_H

#include <QColor>
#include <array>
#include <deque>

#define NUM_TYPE unsigned int

constexpr unsigned int ticksByPeriod = 5;
constexpr NUM_TYPE gridSize = 15;
constexpr unsigned int snakeBaseSize = 4;
constexpr std::array<int, 7> simulationTabSpeed = {500,
                                                   180,
                                                   150,
                                                   120,
                                                   90,
                                                   60,
                                                   30};
const QColor darkGreen = QColor::fromRgb(162, 209, 72);
const QColor appleGreen = QColor::fromRgb(52, 150, 39);
const QColor lightGreen = QColor::fromRgb(169, 215, 80);
const QColor darkBlue = QColor::fromRgb(71, 116, 233);
const QColor darkRed = QColor::fromRgb(231, 71, 29);

typedef std::pair<NUM_TYPE, NUM_TYPE> Coord;
bool operator==(Coord const& lhs, Coord const& rhs);
bool operator!=(Coord const& lhs, Coord const& rhs);
Coord operator+(Coord const& lhs, Coord const& rhs);
Coord operator-(Coord const& lhs, Coord const& rhs);


typedef std::deque<Coord> List;
enum Direction { Up,
                 Down,
                 Left,
                 Right };

constexpr Coord UpCoord{0, -1};
constexpr Coord DownCoord{0, 1};
constexpr Coord LeftCoord{-1, 0};
constexpr Coord RightCoord{1, 0};

[[maybe_unused]] Coord coord(Direction const&);


#endif  // QUETZALCOATL_CONST_H