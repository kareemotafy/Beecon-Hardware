#ifndef TEMP_HUMID_MODULE_H
#define TEMP_HUMID_MODULE_H

#include "DHT.h"

class TempHumidModule {
public:
    TempHumidModule(int dhtPin);
    void initialize();
    void readData(float &humidity, float &temperature);

private:
    int dhtPin;
    DHT dht;
};

#endif  // TEMP_HUMID_MODULE_H
