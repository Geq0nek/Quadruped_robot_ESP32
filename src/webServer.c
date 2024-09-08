#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_http_server.h"

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

static void event_hanlder(void * arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (s_retry_num < WIFI_MAXIMUM_RETRY_NUMBER)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(WIFI_TAG, "Trying to connect to the AP...");
        }
        else 
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(WIFI_TAG, "Failed to connect to the AP...");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(WIFI_TAG, "IP: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void connect_wifi(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_any_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, 
                                                        ESP_EVENT_ANY_ID,
                                                        &event_hanlder,
                                                        NULL,
                                                        &instance_any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, 
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_hanlder,
                                                        NULL,
                                                        &instance_any_ip));

    wifi_config_t wifi_config = 
    {
        .sta = 
        {
            .ssid = WIIF_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(WIFI_TAG, "wIFI_INIT_STA finished");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, 
                                            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                            pdFALSE,
                                            pdFALSE,
                                            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) 
    {
        ESP_LOGI(WIFI_TAG, "Connected to AP SSID: %s Password: %s", WIIF_SSID, WIFI_PASSWORD);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(WIFI_TAG, "Failed to connect to SSID: %s Password: %s", WIIF_SSID, WIFI_PASSWORD);
    } 
    else 
    {
        ESP_LOGI(WIFI_TAG, "Unexpected event");

    }

    vEventGroupDelete(s_wifi_event_group);
}

esp_err_t get_req_handler(httpd_req_t *req)
{
    httpd_resp_send(req, HTML_RESPONSE, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t post_req_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    // READ DATA FROM REQUSET
    if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf) - 1))) <= 0) 
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    buf[ret] = '\0'; // END OF THE STRING

    // PARSING REQUEST
    char *action = strstr(buf, "action="); 
    if (action) 
    {
        action += strlen("action=");
        
        if (strcmp(action, "forward_start") == 0) {
            // MOVE FORWARD
            //startMovingForward();
        } else if (strcmp(action, "backward_start") == 0) {
            // MOVE BACKWARD
            //startMovingBackward();
        } else if (strcmp(action, "left_start") == 0) {
            // MOVE LEFT
            //startTurningLeft();
        } else if (strcmp(action, "right_start") == 0) {
            // MOVE RIGHT
            //startTurningRight();
        } else if (strcmp(action, "stop") == 0) {
            // STOP ROBOT
            //stopMoving();
        }
    }

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t get_uri = 
        {
            .uri = "/",
            .method = HTTP_GET,
            .handler = get_req_handler,
            .user_ctx = NULL
        };

        httpd_uri_t post_uri = 
        {
            .uri = "/control",
            .method = HTTP_POST,
            .handler = post_req_handler,
            .user_ctx = NULL
        };

        httpd_register_uri_handler(server, &get_uri);
        httpd_register_uri_handler(server, &post_uri);
    }
}

void app_main(void)
{
    ESP_LOGI(HTTP_TAG, "Initializing Wifi...");
    connect_wifi();

    ESP_LOGI(HTTP_TAG, "Starting web server...");
    start_webserver();
}