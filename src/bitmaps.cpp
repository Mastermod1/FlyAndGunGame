#include "bitmaps.hpp"

std::uint8_t ship_bitmap[] = {
    0b11111111, 0b11100000, 0b11100000, 0b11111110, 0b11111110, 0b11100000, 0b11100000, 0b11111111,
};

std::uint8_t enemy_bitmap[] = {
    0b00000011, 0b00001111, 0b00011111, 0b11111111, 0b11111111, 0b00011111, 0b00001111, 0b00000011,
};

std::uint8_t bullet_bitmap[] = {
    0b00011000,
    0b00011000,
};

std::uint8_t clear_8x2[2] = {0};
std::uint8_t clear_8x8[8] = {0};
