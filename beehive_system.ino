#include "firebase.h"
#include "temp_humid.h"
#include "mic.h"
#include "weight.h"
#include "camera.h"

// Insert your network credentials
#define WIFI_SSID "Your SSID"
#define WIFI_PASSWORD "Your Password"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCIB8GeCh0hNUihhEy3AKFxrFbvK3YAcYk"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotbeehive-9e6e7-default-rtdb.europe-west1.firebasedatabase.app/"

// Insert sensor pins
#define MIC_PIN 12
#define WEIGHT_DOUT_PIN 13
#define WEIGHT_SCK_PIN 14
#define DHT_PIN 15

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
  setupCamera();

  // Disable WiFi initially
  disableWiFi();
}

void loop()
{
  // Read mic data
  mic_data = MicModule.read_mic();

  // Read humidity and temperature
  temp_humid.readData(humidity_data, temperature_data);

  // Read weight data
  weight_data = WeightModule.getWeight(WEIGHT_numReadings);
  WeightModule.printReadings(WEIGHT_numReadings);

  // Re-enable WiFi
  enableWiFi(WIFI_SSID, WIFI_PASSWORD);
  // Add any necessary WiFi operations here
  Serial.println("Sending to Database...");

  processStream();

  // Store the temperature data in Firebase
  store_sensor_data("temperature", temperature_data);

  // Store the humidity data in Firebase
  store_sensor_data("humidity", humidity_data);

  // Store the mic data in Firebase
  store_sensor_data("sound", mic_data);

  // Store the weight data in Firebase
  store_sensor_data("weight", weight_data);

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