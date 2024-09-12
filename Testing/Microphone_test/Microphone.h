#ifndef Microphone_h
#define Microphone_h

class Microphone{
    public:

    static int16_t* sBuffer;

    static float* readingsWindow;

    static void setup(int sampleRate = 44100);

    static size_t readMic(void* buffer);

};

#endif