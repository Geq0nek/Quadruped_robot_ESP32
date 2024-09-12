#pragma once

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include <driver/i2c.h>
#include <esp_err.h>

#define I2C_MASTER_SCL_IO           41               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           42               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          100000           /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                /*!< I2C master doesn't need buffer */
#define I2C_ADDRESS                 0x40             /*!< I2C address of PCA9685 */

#define PCA9685_MODE1               0x00
#define PCA9685_PRESCALE            0xFE
#define LED0_ON_L                   0x06

static const char *TAG = "PCA9685";

esp_err_t i2c_master_init(void) ;

esp_err_t pca9685_write_byte(uint8_t reg, uint8_t data);

esp_err_t pca9685_init();

esp_err_t pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off);

void start_pca(void);