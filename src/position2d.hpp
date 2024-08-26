#pragma once

#include <bitset>

struct Position2D
{
    int x;
    int y;

    bool operator==(const Position2D& rhs) const noexcept { return x == rhs.x and y == rhs.y; }
};

template <>
struct std::hash<Position2D>
{
    std::size_t operator()(const Position2D& p) const noexcept { return p.y * 31 + p.x; }
};
