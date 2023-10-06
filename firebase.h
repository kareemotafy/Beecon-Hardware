
#ifndef FIREBASE_H
#define FIREBASE_H

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Define firebase auth object
FirebaseAuth auth;
// Define firebase config object
FirebaseConfig config;

// Wifi Connection
void wifiConnect(const char* ssid, const char* password){
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

//Firebase initialization
void firebaseInit(const char* ApiKey, const char* DB_url){
  /* Assign the api key (required) */
  config.api_key = ApiKey;

  /* Assign the RTDB URL (required) */
  config.database_url = DB_url;

  bool signupOK = false;

  /* Sign up with anonymous user */
  // Two empty strings indicate email and password but since it is an anonymous user it is left empty
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  // Connect to Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

#endif