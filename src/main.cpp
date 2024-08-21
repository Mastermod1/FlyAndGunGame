#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

void print(void*)
{
    while (1)
    {
        printf("Task\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
};


extern "C" void app_main()
{
    printf("Hello world!\n");
    xTaskCreate(print, "Task 6", 8192, NULL, 5, NULL);

    while (1)
    {
        printf("main\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    };
}
