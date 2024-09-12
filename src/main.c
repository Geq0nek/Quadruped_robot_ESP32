#include "webServer.h"

extern const char* HTML_RESPONSE;

void app_main() 
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(HTTP_TAG, "Initializing Wifi...");
    connect_wifi();

    ESP_LOGI(HTTP_TAG, "Starting web server...");
    start_webserver();

    start_pca();
}
