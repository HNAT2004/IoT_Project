#include "global.h"

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;
DHT20 dht20;


QueueHandle_t xQueueTemperature = xQueueCreate(5, sizeof(float));
QueueHandle_t xQueueHumidity    = xQueueCreate(5, sizeof(float));

SemaphoreHandle_t xBinarySemaphoreHumi = xSemaphoreCreateBinary();
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();