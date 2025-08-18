#pragma once

#include "esp_log.h"
#include "pcaConfig.h"

#define PWM_MID 1297
#define PWM_UP 455
#define PWM_DOWN 2139  

static const char* MOVE_TAG = "moving";

void move_forward(void);

void move_backward(void);

void move_left(void);

void move_right(void);

void stop_moving(void);

void stand(void);

void sit(void);

static inline uint16_t angle_to_pwm(uint16_t angle);
