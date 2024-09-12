#include "movement.h"

void move_forward(void)
{
    // uint8_t channel = 0; // Kanał 0 dla pierwszego serwa
    // uint16_t on = 0;     // Początek PWM
    // uint16_t off = 307;  // 1.5 ms dla 90 stopni

    // // Ustaw PWM dla serwa
    // esp_err_t ret = pca9685_set_pwm(channel, on, off);
    // if (ret != ESP_OK) 
    // {
    //     ESP_LOGE(TAG, "Failed to set PWM for servo");
    // } 
    // else 
    // {
    //     ESP_LOGI(TAG, "Servo set to 90 degrees");
    // }
    ESP_LOGI(MOVE_TAG, "Moving forward");
}

void move_backward(void)
{
    // for (int a = 90; a <= 0; a--)
    // {
    //     uint16_t pwm_value = (a * (4096 / 90));
    //     ESP_ERROR_CHECK(pca9685_set_pwm(0, 0, pwm_value));
    //     vTaskDelay(20 / portTICK_PERIOD_MS);
    // }
    ESP_LOGI(MOVE_TAG, "Moving backward");
}

void move_left(void)
{
    ESP_LOGI(MOVE_TAG, "Moving left");
}

void move_right(void)
{
    ESP_LOGI(MOVE_TAG, "Moving right");
}

void stop_moving(void)
{
    ESP_LOGI(MOVE_TAG, "Stop");
}