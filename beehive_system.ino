// #include <DHT.h>
#include <Wire.h>

// DHT11 pin
#define DHTPIN 13
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);

// MQ135 pin
#define CO2_PIN  36
// LDR pin
#define  LDR_PIN = 6;

// Variables to store sensor readings
  int temp_adc_val;
float temperature = 0.0;
float humidity = 0.0;
float co2 = 0.0;
int lightLevel = 0;


void setup() {
  Serial.begin(115200);

  // // Initialize DHT sensor
  // dht.begin();

}

void loop() {
  // Read temperature and humidity from DHT11
  // humidity = dht.readHumidity();
  // temperature = dht.readTemperature();
temp_adc_val = analogRead(DHTPIN);
temperature = (temp_adc_val * 4.88);
temperature = (temperature/10);
  // Read CO2 level from MQ135
  co2 = analogRead(CO2_PIN);

  // Read light level from LDR
  int lightLevel = 0;

  // Print sensor readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("CO2 Level: ");
  Serial.print(co2);
  Serial.print(" kg\tLight Level: ");
  Serial.println(lightLevel);

  delay(10000); // Delay for 10 seconds before taking the next reading
}