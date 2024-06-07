#ifndef Weight_Module_h
#define Weight_Module_h

#include <Arduino.h>
#include "HX711.h"

#define WEIGHT_DOUT_PIN 13
#define WEIGHT_SCK_PIN 14

class WeightModule
{
public:
  WeightModule(int doutPin, int sckPin);
  void initialize();
  void calibrateAndTare(float calibrationFactor);
  float getWeight(int numReadings);
  int scaleCalibration(int knownWeight);

private:
  int doutPin;
  int sckPin;
  HX711 scale;
};

#endif
