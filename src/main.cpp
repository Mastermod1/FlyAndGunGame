#include <cstdint>

#include "bitmaps.hpp"
#include "digit_bitmaps.hpp"
#include "entities.hpp"
#include "movement_action_params.hpp"
#include "score_printer.hpp"
#include "ssd1306.h"
#include "tasks.hpp"

extern "C" void app_main()
{
    SSD1306_t dev;
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_contrast(&dev, 0xff);
    ssd1306_clear_screen(&dev, false);

    MovementActionParams params;
    xTaskCreate(inputHandlerTask, "Input handler", 8192, &params, 5, NULL);

    std::uint8_t pos = 0;
    std::uint8_t speed = 5;
    std::uint8_t bitmat_height = 8;

    int score = 0;
    ScorePrinter score_display(dev, G_ALL_DIGITS);
    while (1)
    {
        ssd1306_bitmaps(&dev, 0, pos, clear_8x8, clear_8x8.getWidth(), clear_8x8.getHeight(), false);
        pos = std::max(0, std::min(63 - bitmat_height, pos + (params.x_axis * speed)));
        params.x_axis = 0;
        ssd1306_bitmaps(&dev, 0, pos, ship_bitmap, ship_bitmap.getWidth(), ship_bitmap.getHeight(), false);

        spawnEnemyEveryNFrames(10);
        simulateEnemyMovement(dev);

        G_BULLETS.emplace_back(pos + 3, 9);
        simulateBulletMovement(dev, score);

        score_display.display(score);

        ssd1306_show_buffer(&dev);

        vTaskDelay(33 / portTICK_PERIOD_MS);
    };
}
