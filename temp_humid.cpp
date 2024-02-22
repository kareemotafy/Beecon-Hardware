#include "temp_humid.h"

TempHumidModule::TempHumidModule(int dhtPin) : dhtPin(dhtPin), dht(dhtPin,DHT11) {}

void TempHumidModule::initialize() {
    dht.begin();
}

void TempHumidModule::readData(float &humidity, float &temperature) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();


    // check if any reads failed
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
    } else {
        Serial.print("DHT# Humidity: ");
        Serial.print(humidity);
        Serial.print("%");

        Serial.print("  |  ");

        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("°C ~ ");
    }
}
