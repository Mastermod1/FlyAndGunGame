#include <algorithm>
#include <cstdint>
#include <cstring>

#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_random.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "list"
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

std::uint8_t enemy_bitmap[] = {
    0b00000011, 0b00001111, 0b00011111, 0b11111111, 0b11111111, 0b00011111, 0b00001111, 0b00000011,
};

std::uint8_t bullet_bitmap[] = {
    0b00011000,
    0b00011000,
};

std::uint8_t clear_8x2[2] = {0};
std::uint8_t clear_8x8[8] = {0};

struct Position2D
{
    int x;
    int y;
};

std::list<Position2D> G_BULLETS;
std::list<Position2D> G_ENEMIES;

void simulateEnemyMovement(SSD1306_t& dev)
{
    auto it = G_ENEMIES.begin();
    while (it != G_ENEMIES.end())
    {
        if (it->y < 12)
        {
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x8, 8 * 1, 8, false);
            it = G_ENEMIES.erase(it);
        }
        else
        {
            ssd1306_bitmaps(&dev, it->y, it->x, enemy_bitmap, 8 * 1, 8, false);
            it->y -= 5;
            it++;
        }
    }
}

void simulateBulletMovement(SSD1306_t& dev)
{
    auto it = G_BULLETS.begin();
    while (it != G_BULLETS.end())
    {
        if (it->y > 100)
        {
            ssd1306_bitmaps(&dev, it->y, it->x, clear_8x2, 8 * 1, 2, false);
            it = G_BULLETS.erase(it);
        }
        else
        {
            ssd1306_bitmaps(&dev, it->y, it->x, bullet_bitmap, 8 * 1, 2, false);
            it->y += 5;
            it++;
        }
    }
}

void spawnEnemyEveryNFrames(int N)
{
    static int spawn_delay = 0;
    if (spawn_delay == 0)
    {
        int pos_x = esp_random() % 56;
        G_ENEMIES.emplace_front(pos_x, 100);
    }
    spawn_delay = (spawn_delay + 1) % N;
}

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

        ssd1306_show_buffer(&dev);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    };
}
