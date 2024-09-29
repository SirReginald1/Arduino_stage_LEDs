#ifndef Beat_detector_h
#define Beat_detector_h

#define NB_SAMPLES_MIC 1024
#define NB_SAMPLES_ADC 1024
#define SAMPLING_FREQUENCY_MIC 100000
#define SAMPLING_FREQUENCY_ADC 100000

extern const uint_fast16_t samples;

extern const double samplingFrequency;

/** Value indicating if a kick has been detected. Used for comunication between the cores */
// extern bool isKick; // Depricated

extern float vReal[NB_SAMPLES_MIC];

extern void beatDetectionSetup();

extern void switchFFT(uint8_t periferalCode);

extern void getFFT();

extern void readBeatQueue();

extern void resetBeatDetectionVariables();

float GetMaxFrequencyRange(int bin_start, int bin_end);

extern bool detectBand(double* band, float intensity, unsigned long minPausBetweenCalls, unsigned long &lastCall);

extern bool detectSnare();

extern bool detectKick(unsigned long minPausBetweenCalls);

extern bool detectHiHat();

extern void switchAudioInputOff();


#endif