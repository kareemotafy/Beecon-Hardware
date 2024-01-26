#ifndef FIREBASE_H
#define FIREBASE_H

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

void wifiConnect(const char* ssid, const char* password);
void firebaseInit(const char* apiKey, const char* dbUrl);
void storeSensorData(const char* sensorName, float sensorValue);
void storeTemperatureData(float temperature);

#endif  // FIREBASE_H
