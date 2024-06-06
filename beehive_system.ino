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
#define FAN_PIN 2

// insert weight parameters
#define WEIGHT_calibrationFactor 58
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
bool isCameraOn = false;
float humidity_data, temperature_data, weight_data;
float heat_thresh = FAN_HEAT_THRES, cool_thresh = FAN_COOL_THRES;

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
}

void loop()
{
  isCameraOn = get_bool_value("isCameraOn");
  Serial.print("Is camera on: ");
  Serial.println(isCameraOn);

  if (!isCameraOn)
  {
    // Disable WiFi initially
    disableWiFi();

    delay(100);

    // Read humidity and temperature
    temp_humid.readData(humidity_data, temperature_data);

    // Read mic data
    mic_data = MicModule.read_mic();

    // Read weight data
    weight_data = WeightModule.getWeight(WEIGHT_numReadings);

    // Re-enable WiFi
    enableWiFi(WIFI_SSID, WIFI_PASSWORD);

    //---------RTDB Requests---------//
    Serial.println("Sending to RTDB...");

    heat_thresh = get_float_value("heatThresh");
    cool_thresh = get_float_value("coolThresh");

    Serial.print("Heat thresh: ");
    Serial.println(heat_thresh);
    Serial.print("Cool Thresh: ");
    Serial.println(cool_thresh);

    Fan.run(temperature_data, heat_thresh, cool_thresh);

    // Store the temperature data in Firebase
    store_sensor_data("temperature", temperature_data);

    // Store the humidity data in Firebase
    store_sensor_data("humidity", humidity_data);

    // Store the mic data in Firebase
    store_sensor_data("sound", mic_data);

    // Store the weight data in Firebase
    store_sensor_data("weight", weight_data);
  }

  else
  {
    Serial.println("Camera is on");

    processStream();

    Serial.println("Camera is off");
  }
}