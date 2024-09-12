#include "pcaConfig.h"

esp_err_t i2c_master_init(void) 
{
    i2c_config_t conf = 
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    
    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) 
    {
        return ret;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t pca9685_write_byte(uint8_t reg, uint8_t data) 
{
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

esp_err_t pca9685_init() 
{
    // Set the mode of PCA9685
    esp_err_t ret = pca9685_write_byte(PCA9685_MODE1, 0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize PCA9685");
        return ret;
    }
    return ESP_OK;
}

esp_err_t pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off)
{
    uint8_t reg = LED0_ON_L + 4 * channel;
    esp_err_t ret = pca9685_write_byte(reg, on & 0xFF);
    if (ret != ESP_OK) return ret;
    ret = pca9685_write_byte(reg + 1, on >> 8);
    if (ret != ESP_OK) return ret;
    ret = pca9685_write_byte(reg + 2, off & 0xFF);
    if (ret != ESP_OK) return ret;
    return pca9685_write_byte(reg + 3, off >> 8);
}

void start_pca(void) 
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_ERROR_CHECK(pca9685_init());
}