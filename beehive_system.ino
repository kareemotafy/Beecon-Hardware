#include "firebase.h"
#include "temp_humid.h"
#include "mic.h"
#include "weight.h"
#include <Arduino.h>
#include <base64.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"


// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

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


String Photo2Base64()
{

  camera_fb_t *fb = NULL;

  fb = esp_camera_fb_get();

  if (!fb)
  {

    Serial.println("Camera capture failed");

    return "";
  }

  String imageFile = "data:image/jpeg;base64,";
String encrypt = base64::encode(fb->buf, fb->len);
  


  esp_camera_fb_return(fb);

//  return imageFile;
//Serial.println(encrypt);
return encrypt;
}

String urlencode(String str)

{

  String encodedString = "";

  char c;

  char code0;

  char code1;

  char code2;

  for (int i = 0; i < str.length(); i++)
  {

    c = str.charAt(i);

    if (c == ' ')
    {

      encodedString += '+';
    }
    else if (isalnum(c))
    {

      encodedString += c;
    }
    else
    {

      code1 = (c & 0xf) + '0';

      if ((c & 0xf) > 9)
      {

        code1 = (c & 0xf) - 10 + 'A';
      }

      c = (c >> 4) & 0xf;

      code0 = c + '0';

      if (c > 9)
      {

        code0 = c - 10 + 'A';
      }

      code2 = '\0';

      encodedString += '%';

      encodedString += code0;

      encodedString += code1;

       encodedString+=code2;
    }

    yield();
  }

  return encodedString;
}

void setupCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // init with high specs to pre-allocate larger buffers
  if (psramFound())
  {

    config.frame_size = FRAMESIZE_VGA;

    config.jpeg_quality = 12; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  else
  {

    config.frame_size = FRAMESIZE_SVGA;

    config.jpeg_quality = 12; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QQVGA); // VGA|CIF|QVGA|HQVGA|QQVGA   ( UXGA? SXGA? XGA? SVGA? )
}

void  startStreaming()
{
  String jsonData = "{\"photo\":\"" + Photo2Base64() + "\"}";
  store_camera_data("Camera", jsonData);
}

void setup()
{
  // begin serial communication
  Serial.begin(115200);

  // Wifi Connection
  wifi_connect(WIFI_SSID, WIFI_PASSWORD);

  // initialize Firebase
  firebase_init(API_KEY, DATABASE_URL);



  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  setupCamera();

  // // initialize sensors
  // temp_humid.initialize();
  // WeightModule.initialize();
  // WeightModule.calibrateAndTare(WEIGHT_calibrationFactor);

  // // Disable WiFi initially
  // disableWiFi();
}

void loop()
{
  // // Read mic data
  // mic_data = MicModule.read_mic();

  // // Read humidity and temperature
  // temp_humid.readData(humidity_data, temperature_data);

  // // Read weight data
  // weight_data = WeightModule.getWeight(WEIGHT_numReadings);
  // WeightModule.printReadings(WEIGHT_numReadings);

  // // Re-enable WiFi
  // enableWiFi(WIFI_SSID, WIFI_PASSWORD);

  // // Add any necessary WiFi operations here
  // Serial.println("Sending to Database...");

  // // Store the temperature data in Firebase
  // store_sensor_data("temperature", temperature_data);

  // // Store the humidity data in Firebase
  // store_sensor_data("humidity", humidity_data);

  // // Store the mic data in Firebase
  // store_sensor_data("sound", mic_data);

  // // Store the weight data in Firebase
  // store_sensor_data("weight", weight_data);

  // // Disable WiFi again for analog reading
  // disableWiFi();

  // wait a few seconds between measurements.
  startStreaming();
  
  delay(10000);
}