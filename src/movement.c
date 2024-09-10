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

esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {
        return ret;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t pca9685_write_byte(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t pca9685_init() {
    // Set the mode of PCA9685
    esp_err_t ret = pca9685_write_byte(PCA9685_MODE1, 0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize PCA9685");
        return ret;
    }
    return ESP_OK;
}

esp_err_t pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
    uint8_t reg = LED0_ON_L + 4 * channel;
    esp_err_t ret = pca9685_write_byte(reg, on & 0xFF);
    if (ret != ESP_OK) return ret;
    ret = pca9685_write_byte(reg + 1, on >> 8);
    if (ret != ESP_OK) return ret;
    ret = pca9685_write_byte(reg + 2, off & 0xFF);
    if (ret != ESP_OK) return ret;
    return pca9685_write_byte(reg + 3, off >> 8);
}

void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_ERROR_CHECK(pca9685_init());

    while (1) 
    {
        for (int a = 0; a <= 180; a++){
            uint16_t pwm_value = (a * (4096 / 180));
            ESP_ERROR_CHECK(pca9685_set_pwm(0, 0, pwm_value));
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }

        for (int a = 180; a >= 0; a--){
            uint16_t pwm_value = (a * (4096 / 180));
            ESP_ERROR_CHECK(pca9685_set_pwm(0, 0, pwm_value));
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }

    }
}