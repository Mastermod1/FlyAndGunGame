#include "occupancy_grid.hpp"

std::unordered_map<Position2D, bool> G_OCCUPANCY;

void setArea(Position2D pos, int area_x, int area_y, bool value)
{
    for (int x = 0; x < area_x; x++)
    {
        for (int y = 0; y < area_y; y++)
        {
            G_OCCUPANCY[Position2D(pos.x + x, pos.y + y)] = value;
        }
    }
}

Position2D actualEnemyPosition(Position2D pos)
{
    Position2D res_pos;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            const auto tmp_pos = Position2D(pos.x - x, pos.y - y);
            if (G_OCCUPANCY[tmp_pos]) res_pos = tmp_pos;
        }
    }
    return res_pos;
}
