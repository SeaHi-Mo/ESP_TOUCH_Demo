#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_touch.h"
static char* TAG = "Touch demo";

void app_main(void)
{
    static touch_button_num_t touch_button_num;
    ESP_LOGI(TAG, "Hello world!");
    app_touch_calibration(TOUCH_PAD_NUM9);
    while (1) {
        touch_button_num = app_touch_read_raw_data(TOUCH_PAD_NUM9);

        switch (touch_button_num) {
            case TOUCH_BUTTON_NUM1:
                ESP_LOGI(TAG, "Button:%d", TOUCH_BUTTON_NUM1);
                break;
            case TOUCH_BUTTON_NUM2:
                ESP_LOGI(TAG, "Button:%d", TOUCH_BUTTON_NUM2);
                break;
            case TOUCH_BUTTON_NUM3:
                ESP_LOGI(TAG, "Button:%d", TOUCH_BUTTON_NUM3);
                break;
            case TOUCH_BUTTON_NUM4:
                ESP_LOGI(TAG, "Button:%d", TOUCH_BUTTON_NUM4);
                break;
            default:
                break;
        }
        touch_button_num = TOUCH_BUTTON_ERROR;

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

}
