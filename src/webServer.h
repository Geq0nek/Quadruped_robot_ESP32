#pragma once 

#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "nvs_flash.h"

#include "movement.h"
#include "webBody.c"

// DEFINE MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define WIIF_SSID "PLAY_Swiatlowodowy_99DF"
#define WIFI_PASSWORD "9Zg@7WNB1tcM"
#define WIFI_MAXIMUM_RETRY_NUMBER 3

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *WIFI_TAG = "Wifi Section"; 
static const char *HTTP_TAG = "HTTP Server";

static EventGroupHandle_t s_wifi_event_group;

static int s_retry_num = 0;

void event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void connect_wifi(void);

esp_err_t get_req_handler(httpd_req_t *req);

esp_err_t post_req_handler(httpd_req_t *req);

void start_webserver(void);

// void app_main(void);


