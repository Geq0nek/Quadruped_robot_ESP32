#include "movement.h"

static inline uint16_t angle_to_pwm(uint16_t angle) {
    return (angle * (PWM_DOWN - PWM_UP) / 180) + PWM_UP;
}

static void move(const char* direction, uint8_t channel, uint16_t angle) {
    uint16_t pwm = angle_to_pwm(angle);
    ESP_ERROR_CHECK(pca9685_set_pwm(channel, 0, pwm));
    ESP_LOGI(MOVE_TAG, "Moving %s", direction);
}


void move_forward(void)  { move("forward", 4, 90); }
void move_backward(void) { move("backward", 4, 0); }
void move_left(void)     { move("left", 4, 66); }
void move_right(void)    { move("right", 4, 180); }
void stop_moving(void)   { ESP_LOGI(MOVE_TAG, "Stop"); }

void stand(void) {
    struct { uint8_t channel; uint16_t angle; } cmds[] = {
        {1, 130}, {4, 50}, {7, 130}, {10, 50},
        {0, 50},  {3, 130}, {6, 50},  {9, 130}
    };

    for(int i = 0; i < 8; i++) {
        pca9685_set_pwm(cmds[i].channel, 0, angle_to_pwm(cmds[i].angle));
    }
}

void sit(void) {
    struct { uint8_t channel; uint16_t angle; } cmds[] = {
        {9, 90}, {6, 90}, {10, 90}, {7, 90},
        {0, 90}, {3, 90}, {1, 90}, {4, 90}
    };

    for(int i = 0; i < 8; i++) {
        pca9685_set_pwm(cmds[i].channel, 0, angle_to_pwm(cmds[i].angle));
    }
}
