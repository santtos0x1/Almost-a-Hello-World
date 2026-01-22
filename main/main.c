#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STACK_SIZE 200

gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << 16), 
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};

void vTaskCode( void * pin )
{
    const TickType_t xDelay = 100;

    gpio_num_t pin_cast = *(gpio_num_t *)pin;
    for( ;; )
    {
        gpio_set_level( pin_cast, 1 );
        vTaskDelay( xDelay );
    }
}

void taskBuilder( void )
{
    static uint8_t pin_1 = 16;
    TaskHandle_t xHandle = NULL;

    xTaskCreate( vTaskCode, "Led HIGH", STACK_SIZE, &pin_1, tskIDLE_PRIORITY, &xHandle );
}

void app_main( void )
{
    gpio_config(&io_conf);
    taskBuilder();
}
