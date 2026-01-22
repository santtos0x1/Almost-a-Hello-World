#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STACK_SIZE 200

gpio_config_t led_io_conf = {
    .pin_bit_mask = (1ULL << 16),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};

gpio_config_t btn_io_conf = {
    .pin_bit_mask = (1ULL << 18),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_POSEDGE
};

typedef struct {
    uint8_t led_pin;
    uint8_t btn_pin;
} gpio_pinout;

void vTaskCode( void * pvParameters )
{
    gpio_pinout * pins = ( gpio_pinout * ) pvParameters;
    const TickType_t xDelay = 100;

    for( ;; )
    {
        if(gpio_get_level( ( gpio_num_t )pins->btn_pin ) == 0 )
        {
            gpio_set_level( ( gpio_num_t )pins->led_pin, 1 );
        }
        else
        {
            gpio_set_level( ( gpio_num_t )pins->led_pin, 0 );
        }
        vTaskDelay( xDelay );
    }
}

void taskBuilder( void )
{
    static gpio_pinout pinout;
    pinout.led_pin = 16;
    pinout.btn_pin = 18;

    TaskHandle_t xHandle = NULL;

    xTaskCreate( vTaskCode, "Led HIGH", STACK_SIZE, &pinout, tskIDLE_PRIORITY, &xHandle );
}

void app_main( void )
{
    gpio_config( &led_io_conf );
    gpio_config( &btn_io_conf );
    taskBuilder();
}
