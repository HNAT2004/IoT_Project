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
        glob_temperature = temperature;
        glob_humidity = humidity;

        // Print to Serial
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");

        // Print to LCD
        lcd.setCursor(0,0); 
        lcd.print("Temp: "); lcd.print(temperature); lcd.print(" C   "); 
        lcd.setCursor(0,1); 
        lcd.print("Humi: "); lcd.print(humidity); lcd.print(" %   ");

        xSemaphoreGive(xBinarySemaphoreTemp);
        xSemaphoreGive(xBinarySemaphoreHumi);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
