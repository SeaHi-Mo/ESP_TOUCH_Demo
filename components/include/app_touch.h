#ifndef _APP_TOUCH_H
#define _APP_TOUCH_H

#include "driver/touch_pad.h"
/**
 * @brief 按键枚举，暂时只测试4个触摸按键，您可以自己添加
 *
 */
typedef enum {
    TOUCH_BUTTON_ERROR = 0,
    TOUCH_BUTTON_NUM1,
    TOUCH_BUTTON_NUM2,
    TOUCH_BUTTON_NUM3,
    TOUCH_BUTTON_NUM4,
    TOUCH_BUTTON_NUM_MAX,
}touch_button_num_t;

typedef struct {
    touch_button_num_t button_num;
    uint16_t touch_cap_value_max;
    uint16_t touch_cap_value_cap;  //串联的电容
}touch_button_t;

void app_touch_calibration(touch_pad_t touch_num);
touch_button_num_t app_touch_read_raw_data(touch_pad_t touch_num);
#endif
