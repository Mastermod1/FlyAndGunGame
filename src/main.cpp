#include <algorithm>
#include <cstdint>
#include <string>

#include "entities.hpp"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "movement_action_params.hpp"
#include "ssd1306.h"
#include "tasks.hpp"
#include "bitmaps.hpp"


std::uint8_t digit_0[] = {
    0b00000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00000000,
};

std::uint8_t digit_1[] = {
    0b00000000,
    0b00000000,
    0b00001000,
    0b01000100,
    0b01111110,
    0b01000000,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_2[] = {
    0b00000000,
    0b00000000,
    0b01000100,
    0b01100010,
    0b01010100,
    0b01001000,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_3[] = {
    0b00000000,
    0b00000000,
    0b00100100,
    0b01000010,
    0b01011010,
    0b00100100,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_4[] = {
    0b00000000,
    0b00000000,
    0b00001110,
    0b01001000,
    0b01111000,
    0b01001000,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_5[] = {
    0b00000000,
    0b00000000,
    0b00101110,
    0b01001010,
    0b01001010,
    0b00110010,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_6[] = {
    0b00000000,
    0b00000000,
    0b00110010,
    0b01001010,
    0b01001010,
    0b00111100,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_7[] = {
    0b00000000,
    0b00000000,
    0b01000010,
    0b00100010,
    0b00010010,
    0b00001110,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_8[] = {
    0b00000000,
    0b00000000,
    0b00111100,
    0b01001010,
    0b01010010,
    0b00111100,
    0b00000000,
    0b00000000,
};

std::uint8_t digit_9[] = {
    0b00000000,
    0b00000000,
    0b01001000,
    0b01010100,
    0b00110100,
    0b00011000,
    0b00000000,
    0b00000000,
};

extern "C" void app_main()
{
    SSD1306_t dev;
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
    ssd1306_clear_screen(&dev, false);

    MovementActionParams params;
    xTaskCreate(inputHandlerTask, "Input handler", 8192, &params, 5, NULL);

    std::uint8_t pos = 0;
    std::uint8_t speed = 5;
    std::uint8_t bitmat_height = 8;

    int score = 12;
    while (1)
    {
        pos = std::max(0, std::min(63 - bitmat_height, pos + (params.x_axis * speed)));
        params.x_axis = 0;
        ssd1306_clear_screen(&dev, false);
        ssd1306_bitmaps(&dev, 0, pos, ship_bitmap, 8 * 1, 8, false);

        spawnEnemyEveryNFrames(5);
        simulateEnemyMovement(dev);

        G_BULLETS.emplace_front(pos + 3, 9);
        simulateBulletMovement(dev);

        ssd1306_bitmaps(&dev, 120, pos, digit_0, 8 * 1, 8, false);
        ssd1306_bitmaps(&dev, 120, 8, digit_1, 8 * 1, 8, false);
        ssd1306_show_buffer(&dev);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    };
}
