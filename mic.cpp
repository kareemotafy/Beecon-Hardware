#include "mic.h"
#include <Arduino.h>

MicModule::MicModule(int MIC_PIN) : MIC_PIN(MIC_PIN) {} // sample window set to 50 ms (20 hz) to be set for bees

void MicModule::initialize()
{
  pinMode(MIC_PIN, INPUT);
}

int MicModule::read_mic()
{
  sample = analogRead(MIC_PIN);
  Serial.print("sample: ");
  Serial.println(sample);

  return sample;
}