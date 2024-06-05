#ifndef FIREBASE_H
#define FIREBASE_H

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

void wifi_connect(const char *ssid, const char *password);
void disableWiFi();
void enableWiFi(const char *ssid, const char *password);
void firebase_init(const char *apiKey, const char *dbUrl, const char *projectId);
void store_sensor_data(const char *sensorName, float sensorValue);
void store_camera_data(const char *sensorName, String sensorValue);
bool get_bool_value(const char *topicName);
float get_float_value(const char *topicName);
void firestoreDataUpdate(const char *sensorName, float sensorValue);
String generateRandomString(size_t length);

#endif // FIREBASE_H
