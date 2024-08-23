#include <algorithm>
#include <cstdint>
#include <cstring>

#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_system.h"
#include "font8x8_basic.h"
#include "freertos/FreeRTOS.h"
#include "ssd1306.h"

struct MovementActionParams
{
    int move_left_ = 0;
    int move_right = 0;
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
        if (gpio_left == 0)
            params->move_left_ = 1;
        else
            params->move_left_ = 0;

        if (gpio_right == 0)
            params->move_right = 1;
        else
            params->move_right = 0;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
};

extern "C" void app_main()
{
    printf("Hello world!\n");
    SSD1306_t dev;
    int top;
    char lineChar[20];
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);

    top = 2;
    std::uint8_t image[24] = {0};
    for (int font = 0x39; font > 0x30; font--)
    {
        std::memset(image, 0, sizeof(image));
        ssd1306_display_image(&dev, top + 1, (7 * 8 - 1), image, 8);
        std::memcpy(image, font8x8_basic_tr[font], 8);
        if (dev._flip) ssd1306_flip(image, 8);
        ssd1306_display_image(&dev, top + 1, (7 * 8 - 1), image, 8);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    ssd1306_clear_screen(&dev, false);

    MovementActionParams params;
    xTaskCreate(handleInputs, "Input handler", 8192, &params, 5, NULL);

    int position_x = 0;
    std::uint8_t ship_image[1] = {1};
    while (1)
    {
        ssd1306_clear_screen(&dev, false);
        position_x = std::max(0, std::min(7, position_x + params.move_right - params.move_left_));
        ssd1306_display_image(&dev, position_x, 0, ship_image, 1);

        printf("left: %d, right %d, pos %d\n", params.move_left_, params.move_right, position_x);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    };
}
