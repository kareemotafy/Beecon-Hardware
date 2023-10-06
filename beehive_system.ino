#include <Arduino.h>

// sensors functions
#include "sensors.h"
// firebase functions
#include "firebase.h"

// Insert your network credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBpfYPHwgxLhHvnzblC5zrLvk_4GDuHYKA"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://beecon-4c612-default-rtdb.europe-west1.firebasedatabase.app/"

#define temperatureSensorPin 13

void setup()
{
  // begin serial communication
  Serial.begin(115200);

  // Wifi Connection
  wifiConnect(WIFI_SSID, WIFI_PASSWORD);

  // initialize Firebase
  firebaseInit(API_KEY, DATABASE_URL);
}

void loop()
{

  // Read the temperature from the sensor
  float temperature = analogRead(temperatureSensorPin) * 5.0 / 1024.0;

  // Store the temperature data in Firebase
  storeTemperatureData(temperature);

  // Print the temperature to the serial monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Wait for 1 second before reading the temperature again
  delay(1000);
}