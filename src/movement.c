#include "movement.h"

// SERVO 1
// uint16_t pwm_value = (20 * (4096 / 180)); - 180* GÓRA - 455,(1) ~ 455
// uint16_t pwm_value = (57 * (4096 / 180)); - 90* SRODEK  - 1297,0(6) ~ 1297
// uint16_t pwm_value = (94 * (4096 / 180)); - 0* DÓŁ - 2139,0(2) ~ 2139
// 

// SERVO 2
// uint16_t pwm_value = (20 * (4096 / 180)); - 0* DÓŁ - 455,(1) ~ 455
// uint16_t pwm_value = (57 * (4096 / 180)); - 90* SRODEK - 1297,0(6) ~ 1297
// uint16_t pwm_value = (94 * (4096 / 180)); - 180* GÓRA  - 2139,0(2) ~ 2139

long map(long x)
{
    return (x - 0) * (2139 - 455) / (180 - 0) + 455;
}

void move_forward(void)
{
    uint16_t pwm_value = (57 * (4096 / 180));// 90*
    ESP_ERROR_CHECK(pca9685_set_pwm(4, 0, pwm_value));

    ESP_LOGI(MOVE_TAG, "Moving forward");
}

void move_backward(void)
{

    uint16_t pwm_value = (94 * (4096 / 180)); // 0*
    ESP_ERROR_CHECK(pca9685_set_pwm(4, 0, pwm_value));

    ESP_LOGI(MOVE_TAG, "Moving backward");
}

void move_left(void)
{
    uint16_t pwm_value = (66 * (4096 / 180));
    ESP_ERROR_CHECK(pca9685_set_pwm(4, 0, pwm_value));
    ESP_LOGI(MOVE_TAG, "Moving left");
}

void move_right(void)
{
    uint16_t pwm_value = (20 * (4096 / 180)); //180*
    ESP_ERROR_CHECK(pca9685_set_pwm(4, 0, pwm_value));
    ESP_LOGI(MOVE_TAG, "Moving right");
}

void stop_moving(void)
{
    ESP_LOGI(MOVE_TAG, "Stop");
}

void stand(void)
{   
    pca9685_set_pwm(1, 0, map(130));
    pca9685_set_pwm(4, 0, map(50));
    pca9685_set_pwm(7, 0, map(130));   
    pca9685_set_pwm(10, 0, map(50)); 

    pca9685_set_pwm(0, 0, map(50));
    pca9685_set_pwm(3, 0, map(130));
    pca9685_set_pwm(6, 0, map(50));   
    pca9685_set_pwm(9, 0, map(130));   
}

void sit(void)
{
    pca9685_set_pwm(9, 2139, map(90));
    pca9685_set_pwm(6, 455, map(90));


    pca9685_set_pwm(10, 2139, map(90));
    pca9685_set_pwm(7, 455, map(90));

    pca9685_set_pwm(0, 455, map(90));
    pca9685_set_pwm(3, 2139, map(90));;

    pca9685_set_pwm(1, 455, map(90));
    pca9685_set_pwm(4, 2139, map(90));
    

    // pca9685_set_pwm(1, 0, map(140));
    // pca9685_set_pwm(4, 0, map(40));
    // pca9685_set_pwm(7, 0, map(140));   
    // pca9685_set_pwm(10, 0, map(40)); 

    // pca9685_set_pwm(0, 0, map(90));
    // pca9685_set_pwm(3, 0, map(90));
    // pca9685_set_pwm(6, 0, map(90));   
    // pca9685_set_pwm(9, 0, map(90));   
}
