#include "led_blinky.h"


int ledDelay = 1000;   // mặc định
float recvTemp;

// Hàm xử lý điều kiện nhiệt độ bằng switch-case
void updateLedDelay(void *pvParameters) {
  while (1) {
    if(xQueueReceive(xQueueTemperature, &recvTemp, portMAX_DELAY)) {
      if (recvTemp >= 35) {
        ledDelay = 250;  
      }
      else if (recvTemp >= 30 && recvTemp < 35) {
        ledDelay = 500;    
      }
      else if (recvTemp >= 25 && recvTemp < 30) {
        ledDelay = 1000;    
      }
      else{
        ledDelay = 2000; 
      }
    }
  }
}

void led_blinky(void *pvParameters) {
  Wire.begin(11, 12);
  dht20.begin();
  Serial.begin(115200);
  pinMode(LED_GPIO, OUTPUT);

  while (1) {
    digitalWrite(LED_GPIO, HIGH);
    vTaskDelay(ledDelay);
    digitalWrite(LED_GPIO, LOW);
    vTaskDelay(ledDelay);
  }
}
