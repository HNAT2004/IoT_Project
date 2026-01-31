#include "led_blinky.h"


int ledDelay = 1000;   // mặc định

// Task 1: Single LED Blink with Temperature Conditions
void updateLedDelay(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(xBinarySemaphoreTemp, portMAX_DELAY) == pdTRUE) {
      if (glob_temperature >= 35) {
        ledDelay = 250;  
      }
      else if (glob_temperature >= 30 && glob_temperature < 35) {
        ledDelay = 500;    
      }
      else if (glob_temperature >= 25 && glob_temperature < 30) {
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
