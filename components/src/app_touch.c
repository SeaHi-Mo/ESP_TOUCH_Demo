#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_log.h"
#include "app_touch.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//触摸滤波器的优先级
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)
#define TOUCH_THRESH_NO_USE   (0)

touch_button_t touch_button[] = {
    {TOUCH_BUTTON_NUM1,600,30},
    {TOUCH_BUTTON_NUM2,800,7},
    {TOUCH_BUTTON_NUM3,450,100},
    {TOUCH_BUTTON_NUM4,340,230},
};
/**
 * @brief 初始化触摸按键
 *
 * @param touch_num
 */
static void app_touch_init(touch_pad_t touch_num)
{
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_set_cnt_mode(touch_num, TOUCH_PAD_SLOPE_2, TOUCH_PAD_TIE_OPT_HIGH);
    touch_pad_config(touch_num, TOUCH_THRESH_NO_USE);

    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
}
/**
 * @brief 按键校准
 *
 * @param touch_num
 */
void app_touch_calibration(touch_pad_t touch_num)
{
    uint16_t touch_value;
    uint16_t touch_filter_value;
    uint16_t touch_filter_value_n = 0;
    app_touch_init(touch_num);
    // touch_pad_read_raw_data(touch_num, &touch_value);

    for (size_t i = 0; i < TOUCH_BUTTON_NUM_MAX - 1; i++)
    {
        printf("calibration button:%d\r\n", i + 1);
        while (touch_value > 800) {
            touch_pad_read_filtered(touch_num, &touch_value);
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        for (size_t i = 0; i < 30; i++)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            touch_pad_read_filtered(touch_num, &touch_filter_value);
            touch_filter_value_n += touch_filter_value;
            touch_pad_clear_status();
            if (i == 10 || i == 20 || i == 30)
            {
                printf("%d\r\n", i / 10);
            }

        }
        touch_button[i].touch_cap_value_max = touch_filter_value_n / 30;
        printf("button value:%d\r\n", touch_button[i].touch_cap_value_max);
        touch_filter_value_n = 0;

        touch_pad_read_filtered(touch_num, &touch_value);
        while (touch_value < 1000) {
            touch_pad_read_filtered(touch_num, &touch_value);
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
    }
}

/**
 * @brief 读取触摸按键的值
 *
 * @param touch_num
 */
touch_button_num_t app_touch_read_raw_data(touch_pad_t touch_num)
{
    uint16_t touch_value;
    uint16_t touch_filter_value;
    uint16_t touch_filter_value_n = 0;
    uint16_t touch_value_min = 0;
    double touch_value_x[4] = { 0,0,0,0 };
    static touch_button_num_t touch_button_num = TOUCH_BUTTON_ERROR;

    //  touch_pad_read_raw_data(touch_num, &touch_value);
    for (size_t i = 0; i < 30; i++)
    {
        touch_pad_read_filtered(touch_num, &touch_filter_value);
        touch_filter_value_n += touch_filter_value;
        vTaskDelay(5 / portTICK_PERIOD_MS);
        touch_pad_clear_status();
    }

    touch_filter_value_n /= 30;
    if (touch_filter_value < 1000) {
        //  printf("T%d:[%4d] \r\n", touch_num, touch_filter_value_n);
        for (size_t i = 0; i < TOUCH_BUTTON_NUM_MAX - 1; i++)
        {
            touch_value_x[i] = pow((double)touch_button[i].touch_cap_value_max - touch_filter_value_n, 2.0);
            touch_value_x[i] = sqrt(touch_value_x[i]);
            //  printf("button%d:%.0f   ", i + 1, touch_value_x[i]);
        }
        // printf("\n");
        touch_value_min = touch_value_x[0];
        for (size_t i = 0; i < TOUCH_BUTTON_NUM_MAX - 1; i++)
        {
            if (touch_value_min > touch_value_x[i] && i > 0) {
                touch_value_min = touch_value_x[i];
                touch_button_num = i + 1;
            }
            if (i == 0) touch_button_num = i + 1;
        }
    }
    else touch_button_num = TOUCH_BUTTON_ERROR;
    return touch_button_num;
}

