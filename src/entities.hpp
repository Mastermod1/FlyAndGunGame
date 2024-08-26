#pragma once

#include "list"
#include "position2d.hpp"
#include "ssd1306.h"

extern std::list<Position2D> G_BULLETS;
extern std::list<Position2D> G_ENEMIES;

void simulateEnemyMovement(SSD1306_t& dev);
void simulateBulletMovement(SSD1306_t& dev);

void spawnEnemyEveryNFrames(int N);
