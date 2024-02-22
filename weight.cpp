#include "weight.h"

WeightModule::WeightModule(int doutPin, int sckPin)
    : doutPin(doutPin), sckPin(sckPin) {}

void WeightModule::initialize() {
  // Serial.begin(57600);
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  scale.begin(doutPin, sckPin);
}

void WeightModule::printScaleInfo() {
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));
  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));
  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);
}

void WeightModule::calibrateAndTare(float calibrationFactor) {
  scale.set_scale(calibrationFactor);
  scale.tare();
}

float WeightModule::getWeight(int numReadings) {
  return scale.get_units(numReadings);
}

void WeightModule::printReadings(int numReadings) {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(numReadings), 5);
}

int WeightModule::scaleCalibration(int knownWeight) {
  float calibrationFactor = 0;
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
    calibrationFactor = reading / knownWeight;
    Serial.print("Calibration Factor: ");
    Serial.println(calibrationFactor);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
  return calibrationFactor;
}

