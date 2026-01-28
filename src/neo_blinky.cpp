#include "neo_blinky.h"


void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {
        if (xSemaphoreTake(xBinarySemaphoreHumi, portMAX_DELAY) == pdTRUE) {       
            if(glob_humidity > 80){
                strip.setPixelColor(0, strip.Color(0, 0, 255)); 
            }
            else if(glob_humidity > 20 && glob_humidity <= 80) {
                strip.setPixelColor(0, strip.Color(0, 255, 0)); 
            }
            else{
                strip.setPixelColor(0, strip.Color(255, 0, 0));
            }            
            strip.show(); 
        }
    }
}