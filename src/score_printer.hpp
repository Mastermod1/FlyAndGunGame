#pragma once

#include <cmath>
#include <cstdint>

#include "ssd1306.h"

class ScorePrinter
{
  public:
    ScorePrinter(SSD1306_t& dev, std::uint8_t* digits[10]) : dev(dev), digits(digits) {}
    void display(int score)
    {
        for (int i = 9; i >= 0; i--)
        {
            int power = pow(10, i);
            int index = score / power;
            score %= power;
            ssd1306_bitmaps(&dev, 120, (9 - i) * 5, digits[index], 8 * 1, 4, false);
        }
    }

    SSD1306_t& dev;
    std::uint8_t** digits;
};
