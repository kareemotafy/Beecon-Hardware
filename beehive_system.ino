#include "firebase.h"
#include "temp_humid.h"
#include "mic.h"
#include "weight.h"
#include "fan.h"
#include "camera.h"

// Insert your network credentials
#define WIFI_SSID "Your SSID"
#define WIFI_PASSWORD "Your Password"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCIB8GeCh0hNUihhEy3AKFxrFbvK3YAcYk"
#define PROJECT_ID "iotbeehive-9e6e7"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotbeehive-9e6e7-default-rtdb.europe-west1.firebasedatabase.app/"

// Insert sensor pins
#define MIC_PIN 12
#define WEIGHT_DOUT_PIN 13
#define WEIGHT_SCK_PIN 14
#define DHT_PIN 15
#define FAN_PIN 16

// insert weight parameters
#define WEIGHT_calibrationFactor 6.36
#define WEIGHT_numReadings 10

// insert fan parameters
#define FAN_HEAT_THRES 20
#define FAN_COOL_THRES 35

// Define sensor classes
TempHumidModule temp_humid(DHT_PIN);
MicModule MicModule(MIC_PIN);
WeightModule WeightModule(WEIGHT_DOUT_PIN, WEIGHT_SCK_PIN);
Fan Fan(FAN_PIN);

int mic_data = 0;
float humidity_data, temperature_data, weight_data;

void setup()
{
  // begin serial communication
  Serial.begin(115200);

  // Wifi Connection
  wifi_connect(WIFI_SSID, WIFI_PASSWORD);

  // initialize Firebase
  firebase_init(API_KEY, DATABASE_URL, PROJECT_ID);

  // initialize sensors
  temp_humid.initialize();
  MicModule.initialize();
  WeightModule.initialize();
  WeightModule.calibrateAndTare(WEIGHT_calibrationFactor);
  setupCamera();

  // initialize actuators
  Fan.initialize();

  // Disable WiFi initially
  disableWiFi();
}

void loop()
{
  // Read mic data
  mic_data = MicModule.read_mic();

  // Read humidity and temperature
  temp_humid.readData(humidity_data, temperature_data);
  Fan.run(temperature_data, FAN_HEAT_THRES, FAN_COOL_THRES);

  // Read weight data
  weight_data = WeightModule.getWeight(WEIGHT_numReadings);
  WeightModule.printReadings(WEIGHT_numReadings);

  // Re-enable WiFi
  enableWiFi(WIFI_SSID, WIFI_PASSWORD);

  //---------RTDB Requests---------//
  Serial.println("Sending to RTDB...");

  processStream();

  // Store the temperature data in Firebase
  store_sensor_data("temperature", temperature_data);

  // Store the humidity data in Firebase
  store_sensor_data("humidity", humidity_data);

  // Store the mic data in Firebase
  store_sensor_data("sound", mic_data);

  // Store the weight data in Firebase
  store_sensor_data("weight", weight_data);

  //---------Firestore Requests---------//
  Serial.println("Sending to Firestore...");

  firestoreDataUpdate("sound", mic_data);

  firestoreDataUpdate("temperature", temperature_data);

  firestoreDataUpdate("humidity", humidity_data);

  firestoreDataUpdate("weight", weight_data);

  // delays sensor reading by 5 seconds
  // updates the stream every 500 milliseconds
  for (int i = 0; i < 10; i++)
  {
    processStream();
    delay(500);
  }

  // Disable WiFi again for analog reading
  disableWiFi();
}