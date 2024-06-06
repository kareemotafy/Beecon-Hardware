#include "weight.h"

WeightModule::WeightModule(int doutPin, int sckPin)
    : doutPin(doutPin), sckPin(sckPin) {}

void WeightModule::initialize()
{
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  scale.begin(doutPin, sckPin);
}

void WeightModule::calibrateAndTare(float calibrationFactor)
{
  scale.set_scale(calibrationFactor);
  scale.tare();
}

float WeightModule::getWeight(int numReadings)
{
  float weight = scale.get_units(numReadings);
  Serial.print("Weight = ");
  Serial.println(weight);

  return weight;
}

int WeightModule::scaleCalibration(int knownWeight)
{
  float calibrationFactor = 0;
  if (scale.is_ready())
  {
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
  else
  {
    Serial.println("HX711 not found.");
  }
  delay(1000);
  return calibrationFactor;
}
