#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

#include "ssd1306.h"
#include "font8x8_basic.h"

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
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
};


extern "C" void app_main()
{
    printf("Hello world!\n");
    SSD1306_t dev;
	int center, top, bottom;
	char lineChar[20];
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);

    MovementActionParams params;
    xTaskCreate(handleInputs, "Input handler", 8192, &params, 5, NULL);

    while (1)
    {
        printf("left: %d, right %d\n", params.move_left_, params.move_right);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    };
}
