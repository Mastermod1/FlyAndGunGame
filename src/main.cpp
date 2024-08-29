#include <cstdint>

#include "bitmaps.hpp"
#include "entities.hpp"
#include "movement_action_params.hpp"
#include "ssd1306.h"
#include "tasks.hpp"
#include "score_printer.hpp"
#include "digit_bitmaps.hpp"

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

    int score = 0;
    ScorePrinter score_display(dev, G_ALL_DIGITS);
    while (1)
    {
        pos = std::max(0, std::min(63 - bitmat_height, pos + (params.x_axis * speed)));
        params.x_axis = 0;
        ssd1306_clear_screen(&dev, false);
        ssd1306_bitmaps(&dev, 0, pos, ship_bitmap, 8 * 1, 8, false);

        spawnEnemyEveryNFrames(5);
        simulateEnemyMovement(dev);

        G_BULLETS.emplace_front(pos + 3, 9);
        simulateBulletMovement(dev, score);

        score_display.display(score);

        ssd1306_show_buffer(&dev);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    };
}
