#ifndef MIC_MODULE_H
#define MIC_MODULE_H

class MicModule
{
public:
    MicModule(int MIC_PIN);
    void initialize();
    int read_mic();

private:
    const int MIC_PIN;
    unsigned int sample;
};

#endif // MIC_MODULE_H