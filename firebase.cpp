#include "firebase.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseAuth auth;
FirebaseConfig config;


void wifi_connect(const char* ssid, const char* password) {
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

void disableWiFi() {
  WiFi.disconnect(true); // Disconnect WiFi and release resources
  WiFi.mode(WIFI_OFF);   // Turn off WiFi module
  Serial.println("WiFi disabled");
}

void enableWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);  // Enable WiFi module
  WiFi.begin(ssid, password); // Reconnect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Reconnecting to WiFi...");
  }
  Serial.println("WiFi reconnected!");
}

void firebase_init(const char* apiKey, const char* dbUrl) {
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
void store_sensor_data(const char* sensorName, float sensorValue) {
  FirebaseData fbdo;

  // Create a JSON object to store the sensor data
  FirebaseJson sensorNode;
  sensorNode.set("sensorData", sensorValue);
  sensorNode.set("timestamp", millis());

  // Check if the sensor node is pushed to Firebase 
  if (Firebase.RTDB.pushJSON(&fbdo, String("/") + sensorName, &sensorNode)) {
    Serial.print(sensorName);
    Serial.println(" pushed to Firebase");
  } 
  else {
    Serial.println(fbdo.errorReason());
  }
}