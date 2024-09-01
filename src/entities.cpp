#include "entities.hpp"

#include "bitmaps.hpp"
#include "esp_random.h"
#include "occupancy_grid.hpp"
#include "ssd1306.h"

std::list<Position2D> G_BULLETS;
std::list<Position2D> G_ENEMIES;

static int G_ENEMY_SPEED = 1;
static int G_BULLET_SPEED = 8;

void simulateEnemyMovement(SSD1306_t& dev)
{
    auto it = G_ENEMIES.begin();
    while (it != G_ENEMIES.end())
    {
        if (it->y < 12)
        {
            setArea(*it, enemy_bitmap.getWidth(), enemy_bitmap.getHeight(), false);
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x8, clear_8x8.getWidth(), clear_8x8.getHeight(), false);
            it = G_ENEMIES.erase(it);
        }
        else
        {
            setArea(*it, enemy_bitmap.getWidth(), enemy_bitmap.getHeight(), false);
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x8, clear_8x8.getWidth(), clear_8x8.getHeight(), false);
            it->y -= G_ENEMY_SPEED;
            ssd1306_bitmaps(&dev, it->y, it->x, enemy_bitmap, enemy_bitmap.getWidth(), enemy_bitmap.getHeight(), false);
            setArea(*it, enemy_bitmap.getWidth(), enemy_bitmap.getHeight(), true);
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
            const auto& actual_position = actualEnemyPosition(*it);
            ssd1306_bitmaps(&dev, actual_position.y, actual_position.x, clear_8x8, clear_8x8.getWidth(), clear_8x8.getHeight(), false);
            G_ENEMIES.remove(actual_position);
            score++;
            it = G_BULLETS.erase(it);
            continue;
        }
        if (it->y > 100)
        {
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x2, clear_8x2.getWidth(), clear_8x2.getHeight(), false);
            it = G_BULLETS.erase(it);
        }
        else
        {
            _ssd1306_pixel(&dev, it->y, it->x, true);
            _ssd1306_pixel(&dev, it->y+1, it->x, true);
            _ssd1306_pixel(&dev, it->y, it->x+1, true);
            _ssd1306_pixel(&dev, it->y+1, it->x+1, true);
            it->y += G_BULLET_SPEED;
            _ssd1306_pixel(&dev, it->y, it->x, false);
            _ssd1306_pixel(&dev, it->y+1, it->x, false);
            _ssd1306_pixel(&dev, it->y, it->x+1, false);
            _ssd1306_pixel(&dev, it->y+1, it->x+1, false);
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
