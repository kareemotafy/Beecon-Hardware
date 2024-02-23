#ifndef FAN_H
#define FAN_H

class Fan {
public:
    Fan(int FAN_PIN);
    void initialize();
    void run(float temp, float heatThreshold, float coolThreshold);

private:
    const int FAN_PIN;
};

#endif // FAN_H
