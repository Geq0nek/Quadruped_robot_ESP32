#pragma once

#include "esp_log.h"
#include "pcaConfig.h"

static const char* MOVE_TAG = "moving";

void move_forward(void);

void move_backward(void);

void move_left(void);

void move_right(void);

void stop_moving(void);