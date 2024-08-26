#pragma once

#include "position2d.hpp"
#include "unordered_map"

extern std::unordered_map<Position2D, bool> G_OCCUPANCY;

void setArea(Position2D pos, int area_x, int area_y, bool value);
Position2D actualEnemyPosition(Position2D pos);
