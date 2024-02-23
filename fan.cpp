#include "fan.h"
#include <Arduino.h>

Fan::Fan(int FAN_PIN) : FAN_PIN(FAN_PIN) {}

void Fan::initialize() {
  pinMode(FAN_PIN, OUTPUT);
}


void Fan::run(float temp, float heatThreshold, float coolThreshold) {
  if(temp > coolThreshold){
    digitalWrite(FAN_PIN, HIGH); 
  }
  else if(temp < heatThreshold){
    digitalWrite(FAN_PIN, HIGH); 
  }
  else{
    digitalWrite(FAN_PIN, LOW); 
  }
}

