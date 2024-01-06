#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/task.h"

#define SWITCH_PIN 13

unsigned long int previousTime = 0;

int switchState = 0;
int prevSwitchState = 0;

long int interval = 599990/portTICK_PERIOD_MS;

int leds[6] = {27, 26, 25, 33, 32, 14};

void app_main(void)
{
    // void setup
    for (int i = 0; i < sizeof(leds) / sizeof(int); i++) {
        gpio_reset_pin(leds[i]);
        gpio_set_direction(leds[i], GPIO_MODE_OUTPUT);
    }

    gpio_reset_pin(SWITCH_PIN);
    gpio_set_direction(SWITCH_PIN, GPIO_MODE_INPUT);

    int led_indx = 0;

    // void loop
    while (1) {
        unsigned long int currentTime = xTaskGetTickCount();

        if (currentTime - previousTime > interval && led_indx != sizeof(leds) / sizeof(int)) {
            previousTime = currentTime;

            gpio_set_level(leds[led_indx], 1);
            led_indx++;

            if (led_indx == sizeof(leds) / sizeof(int)) {
                ESP_LOGI("digital_hourglass", "Time is up!\n");
            }
        }

        switchState = gpio_get_level(SWITCH_PIN);

        if (switchState != prevSwitchState) {
            for(int i = 0; i < sizeof(leds) / sizeof(int); i++) {
                gpio_set_level(leds[i], 0);
            }

            led_indx = 0;
            previousTime = currentTime;
        }

        prevSwitchState = switchState;

        // Delay for cpu housekeeping tasks
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
