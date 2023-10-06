
#ifndef SENSORS_H
#define SENSORS_H

//include Firebase SDK for ESP32
#include <Firebase_ESP_Client.h>

//Define Firebase Data object
FirebaseData fbdo;

// Generic function to store sensor data in Firebase
void storeSensorData(const char* sensorName, float sensorValue) {
  // Create a JSON object to store the sensor data
  FirebaseJson sensorDataJson;

  // Add sensor name and sensor value as two fields of the json
  sensorDataJson.set("sensorName", sensorName);
  sensorDataJson.set("sensorValue", sensorValue);

  // Set the timestamp for the sensor data
  FirebaseJson timestampJson;
  timestampJson.set("timestamp", millis());

  // Create a parent node for the timestamp and sensor data
  FirebaseJson sensorNode;
  sensorNode.set("timestamp", timestampJson);
  sensorNode.set("sensorData", sensorDataJson);


  // Check if the sensor node is pushed to Firebase 
  if (Firebase.RTDB.pushJSON(&fbdo, String("/") + sensorName, &sensorNode)) {
    Serial.println("Data pushed to Firebase");
  } 
  else {
    Serial.println(fbdo.errorReason());
  }
}

// Function to store temperature data in Firebase
void storeTemperatureData(float temperature) {
  // Store the temperature data using the generic store function
  storeSensorData("temperature", temperature);
}


#endif