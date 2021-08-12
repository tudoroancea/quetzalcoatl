//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "global.h"

bool operator==(Coord const& lhs, Coord const& rhs) { return lhs.first == rhs.first && lhs.second == rhs.second; }
bool operator!=(Coord const& lhs, Coord const& rhs) { return !(lhs == rhs); }