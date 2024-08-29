#include "entities.hpp"

#include "bitmaps.hpp"
#include "esp_random.h"
#include "occupancy_grid.hpp"
#include "ssd1306.h"

std::list<Position2D> G_BULLETS;
std::list<Position2D> G_ENEMIES;

void simulateEnemyMovement(SSD1306_t& dev)
{
    auto it = G_ENEMIES.begin();
    while (it != G_ENEMIES.end())
    {
        if (it->y < 12)
        {
            setArea(*it, 8, 8, false);
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x8, 8 * 1, 8, false);
            it = G_ENEMIES.erase(it);
        }
        else
        {
            setArea(*it, 8, 8, false);
            ssd1306_bitmaps(&dev, it->y, it->x, enemy_bitmap, 8 * 1, 8, false);
            it->y -= 5;
            setArea(*it, 8, 8, true);
            it++;
        }
    }
}

void simulateBulletMovement(SSD1306_t& dev, int& score)
{
    auto it = G_BULLETS.begin();
    while (it != G_BULLETS.end())
    {
        if (G_OCCUPANCY[Position2D(it->x, it->y)])
        {
            setArea(*it, 8, 8, false);
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x8, 8 * 1, 8, false);
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x2, 8 * 1, 2, false);
            G_ENEMIES.remove(actualEnemyPosition(*it));
            score++;
            it = G_BULLETS.erase(it);
            continue;
        }
        if (it->y > 100)
        {
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x2, 8 * 1, 2, false);
            it = G_BULLETS.erase(it);
        }
        else
        {
            ssd1306_bitmaps(&dev, it->y, it->x, bullet_bitmap, 8 * 1, 2, false);
            it->y += 5;
            it++;
        }
    }
}

void spawnEnemyEveryNFrames(int N)
{
    static int spawn_delay = 0;
    if (spawn_delay == 0)
    {
        int pos_x = esp_random() % 56;
        setArea(Position2D(pos_x, 100), 8, 8, true);
        G_ENEMIES.emplace_front(pos_x, 100);
    }
    spawn_delay = (spawn_delay + 1) % N;
}
