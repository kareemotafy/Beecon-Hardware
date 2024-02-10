#ifndef Weight_Module_h
#define Weight_Module_h

#include <Arduino.h>
#include "HX711.h"

class WeightModule {
public:
  WeightModule(int doutPin, int sckPin);
  void initialize();
  void printScaleInfo();
  void calibrateAndTare(float calibrationFactor);
  float getWeight(int numReadings);
  void printReadings(int numReadings);
  int scaleCalibration(int knownWeight);


private:
  int doutPin;
  int sckPin;
  HX711 scale;
};

#endif
