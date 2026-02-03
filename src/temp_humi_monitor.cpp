#include "temp_humi_monitor.h"
LiquidCrystal_I2C lcd(33,16,2);

void temp_humi_monitor(void *pvParameters){
    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    while (1){
        dht20.read();
        float temperature = dht20.getTemperature();
        float humidity = dht20.getHumidity();


        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity = -1;
        }

        // Update global variables
        xQueueSend(xQueueTemperature, &temperature, portMAX_DELAY);
        xQueueSend(xQueueHumidity, &humidity, portMAX_DELAY);

        String state;
        if (temperature < 0 || humidity < 0) {
            state = "UNKNOWN";
        }
        else if (temperature >= 35 || humidity > 80) {
            state = "CRITICAL";
        }
        else if ((temperature >= 30 && temperature < 35) || humidity <= 20) {
            state = "WARNING";
        }
        else if (temperature >= 25 && temperature < 30 && humidity > 20 && humidity <= 80) {
            state = "NORMAL";
        }
        else {
            state = "UNKNOWN";
        }

        // Print to Serial
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");

        // Print to LCD
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print(state);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
