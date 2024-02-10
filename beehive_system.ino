#include "firebase.h"
#include "temp_humid.h"
#include "mic.h"
#include "weight.h"

// Insert your network credentials
#define WIFI_SSID "Your SSID"
#define WIFI_PASSWORD "Your Password"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCIB8GeCh0hNUihhEy3AKFxrFbvK3YAcYk"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotbeehive-9e6e7-default-rtdb.europe-west1.firebasedatabase.app/"

// Insert sensor pins
#define DHT_PIN 16
#define MIC_PIN 12
#define WEIGHT_DOUT_PIN 13
#define WEIGHT_SCK_PIN 14

// insert mic sample window
#define MIC_sampleWindow 50

// insert weight parameters
#define WEIGHT_calibrationFactor 6.36
#define WEIGHT_numReadings 10

// Define sensor classes
TempHumidModule temp_humid(DHT_PIN);
MicModule MicModule(MIC_sampleWindow, MIC_PIN);
WeightModule WeightModule(WEIGHT_DOUT_PIN, WEIGHT_SCK_PIN);

int mic_data = 0;
float humidity_data, temperature_data, weight_data;

void setup()
{
  // begin serial communication
  Serial.begin(115200);

  // Wifi Connection
  wifi_connect(WIFI_SSID, WIFI_PASSWORD);

  // initialize Firebase
  firebase_init(API_KEY, DATABASE_URL);

  // initialize sensors
  temp_humid.initialize();
  WeightModule.initialize();
  WeightModule.calibrateAndTare(WEIGHT_calibrationFactor);

  // Disable WiFi initially
  disableWiFi();
}

void loop()
{
  // Read mic data
  mic_data = MicModule.read_mic();

  // wait a few seconds between measurements.
  delay(1000);

  // Read humidity and temperature
  temp_humid.readData(humidity_data, temperature_data);

  // wait a few seconds between measurements.
  delay(1000);

  // Read weight data
  weight_data = WeightModule.getWeight(WEIGHT_numReadings);
  WeightModule.printReadings(WEIGHT_numReadings);

  // Re-enable WiFi
  enableWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Add any necessary WiFi operations here
  Serial.println("hokololo sending...");

  // // Store the mic data in Firebase
  store_sensor_data("sound", mic_data);

  // wait a few seconds between measurements.
  delay(2000);

  // Store the temperature data in Firebase
  store_sensor_data("temperature", temperature_data);

  // wait a few seconds between measurements.
  delay(2000);

  // // Store the humidity data in Firebase
  store_sensor_data("humidity", humidity_data);

  // wait a few seconds between measurements.
  delay(2000);

  // Store the weight data in Firebase
  store_sensor_data("weight", weight_data);

  // Disable WiFi again for analog reading
  disableWiFi();
}
