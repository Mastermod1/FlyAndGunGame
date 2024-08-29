#include "digit_bitmaps.hpp"

std::uint8_t digit_0[] = {
    0b01111110,
    0b01000010,
    0b01000010,
    0b01111110,
};

std::uint8_t digit_1[] = {
    0b00001000,
    0b01000100,
    0b01111110,
    0b01000000,
};

std::uint8_t digit_2[] = {
    0b01000100,
    0b01100010,
    0b01010100,
    0b01001000,
};

std::uint8_t digit_3[] = {
    0b00100100,
    0b01000010,
    0b01011010,
    0b00100100,
};

std::uint8_t digit_4[] = {
    0b00001110,
    0b01001000,
    0b01111000,
    0b01001000,
};

std::uint8_t digit_5[] = {
    0b00101110,
    0b01001010,
    0b01001010,
    0b00110010,
};

std::uint8_t digit_6[] = {
    0b00111110,
    0b01001010,
    0b01001010,
    0b00110000,
};

std::uint8_t digit_7[] = {
    0b01000010,
    0b00100010,
    0b00010010,
    0b00001110,
};

std::uint8_t digit_8[] = {
    0b00111100,
    0b01001010,
    0b01010010,
    0b00111100,
};

std::uint8_t digit_9[] = {
    0b01001000,
    0b01010100,
    0b00110100,
    0b00011000,
};

std::uint8_t* G_ALL_DIGITS[10] = {digit_0, digit_1, digit_2, digit_3, digit_4,
                                  digit_5, digit_6, digit_7, digit_8, digit_9};
