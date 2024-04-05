#include "mic.h"
#include <Arduino.h>

MicModule::MicModule(int sampleWindow, int MIC_PIN) : sampleWindow(sampleWindow), MIC_PIN(MIC_PIN) {} // sample window set to 50 ms (20 hz) to be set for bees


void MicModule::initialize() {
  pinMode(MIC_PIN, INPUT);
}

int MicModule::read_mic()
{
    unsigned long startMillis = millis(); // Start of sample window
    unsigned int peakToPeak = 0;          // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS and then plot data
    while (millis() - startMillis < 50)
    {
        sample = analogRead(MIC_PIN);
        if (sample < 1024) // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample; // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample; // save just the min levels
            }
        }
    }
    peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
    Serial.println(peakToPeak);
    return peakToPeak;
}
