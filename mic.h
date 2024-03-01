#ifndef MIC_MODULE_H
#define MIC_MODULE_H

class MicModule
{
public:
    MicModule(int sampleWindow, int MIC_PIN);
    int read_mic();

private:
    const int sampleWindow;
    const int MIC_PIN;
    unsigned int sample;
};

#endif // MIC_MODULE_H
