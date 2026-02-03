#include "neo_blinky.h"

float recvHumi;

void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {
        if(xQueueReceive(xQueueHumidity, &recvHumi, portMAX_DELAY)) {       
            if(recvHumi > 80){
                strip.setPixelColor(0, strip.Color(0, 0, 255)); 
            }
            else if(recvHumi > 20 && recvHumi <= 80) {
                strip.setPixelColor(0, strip.Color(0, 255, 0)); 
            }
            else{
                strip.setPixelColor(0, strip.Color(255, 0, 0));
            }            
            strip.show(); 
        }
    }
}