#include "tasks.hpp"
#include "movement_action_params.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

void inputHandlerTask(void* movement_action_params)
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
