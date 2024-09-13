#ifndef Microphone_h
#define Microphone_h
#include <driver/i2s.h>
// Define input buffer length
#define bufferLen 1024

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

class Microphone{
    public:

    static void setup(int sampleRate = 44100);

    //static size_t readMic(void* buffer);

    template<typename T> static size_t readMic(void* buffer){
    // Get I2S data and place in data buffer
    size_t bytesIn = 0;
    i2s_read(I2S_PORT, buffer, bufferLen * sizeof(T), &bytesIn, portMAX_DELAY);
    return bytesIn;
}

};

#endif