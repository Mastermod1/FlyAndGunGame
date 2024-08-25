#include <algorithm>
#include <cstdint>
#include <cstring>

#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "ssd1306.h"

struct MovementActionParams
{
    int x_axis = 0;
};

void handleInputs(void* movement_action_params)
{
    MovementActionParams* params = static_cast<MovementActionParams*>(movement_action_params);
    gpio_set_direction(GPIO_NUM_6, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_7, GPIO_MODE_INPUT);

    int gpio_left = 0;
    int gpio_right = 0;
    while (1)
    {
        gpio_left = gpio_get_level(GPIO_NUM_7);
        gpio_right = gpio_get_level(GPIO_NUM_6);

        if (gpio_left == 0) params->x_axis = -1;
        if (gpio_right == 0) params->x_axis = 1;

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
};

std::uint8_t ship_bitmap[] = {
    0b11111111, 0b11100000, 0b11100000, 0b11111110, 0b11111110, 0b11100000, 0b11100000, 0b11111111,
};

std::uint8_t bullet_bitmap[] = {
    0b00011000,
    0b00011000,
};

struct Bullets
{
    int index = 0;
    int max_size = 20;
    std::pair<int, int> bulltets[20] = {{0, 0}};
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
    xTaskCreate(handleInputs, "Input handler", 8192, &params, 5, NULL);

    std::uint8_t pos = 0;
    std::uint8_t speed = 5;
    std::uint8_t bitmat_height = 8;

    Bullets bullets;
    while (1)
    {
        pos = std::max(0, std::min(63 - bitmat_height, pos + (params.x_axis * speed)));
        params.x_axis = 0;
        ssd1306_clear_screen(&dev, false);
        ssd1306_bitmaps(&dev, 0, pos, ship_bitmap, 8 * 1, 8, false);
        bullets.bulltets[bullets.index] = {9, pos + 3};
        bullets.index = (bullets.index + 1) % bullets.max_size;
        for (int i = 0; i < bullets.max_size; i++)
        {
            if (bullets.bulltets[i] != std::pair<int, int>{0, 0})
            {
                ssd1306_bitmaps(&dev, bullets.bulltets[i].first, bullets.bulltets[i].second, bullet_bitmap, 8 * 1, 2,
                                false);
                bullets.bulltets[i].first += 5;
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    };
}
