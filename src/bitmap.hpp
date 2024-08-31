#pragma once

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <new>

class Bitmap
{
  public:
    // Bitmaps oriented from left to right treaded as bottom to top
    Bitmap(std::initializer_list<std::initializer_list<std::uint8_t>> bitmap)
    {
        height_ = bitmap.size();
        width_ = bitmap.begin()->size() * 8;
        bitmap_ = new std::uint8_t[width_ * height_];
        int i = 0;
        for (const auto& row : bitmap)
        {
            for (const auto& col : row)
            {
                bitmap_[i++] = col;
            }
        }
    }

    Bitmap(int width, int height, std::uint8_t val)
    {
        width_ = width;
        height_ = height;
        int memory_width = width_ / 8;
        bitmap_ = new std::uint8_t[memory_width * height_];
        std::memset(bitmap_, val, memory_width * height_);
    }

    Bitmap(const Bitmap& rhs) = delete;
    Bitmap& operator=(const Bitmap& rhs) = delete;
    Bitmap(Bitmap&& rhs) noexcept = delete;
    Bitmap& operator=(Bitmap&& rhs) noexcept = delete;

    int getWidth() { return width_; }
    int getHeight() { return height_; }

    operator std::uint8_t*() { return bitmap_; }

    ~Bitmap() { delete[] bitmap_; }

  private:
    std::uint8_t* bitmap_;
    int width_;
    int height_;
};
