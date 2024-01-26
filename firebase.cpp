#include "firebase.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseAuth auth;
FirebaseConfig config;
FirebaseData fbdo;

void wifiConnect(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void firebaseInit(const char* apiKey, const char* dbUrl) {
  config.api_key = apiKey;
  config.database_url = dbUrl;

  bool signupOK = false;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

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