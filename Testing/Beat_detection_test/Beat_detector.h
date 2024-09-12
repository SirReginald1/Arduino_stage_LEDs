#ifndef Beat_detector_h
#define Beat_detector_h

#define NB_SAMPLES 1024

extern const uint_fast16_t samples;

extern float vReal[NB_SAMPLES];

extern void beatDetectionSetup();

extern void getFFT();

float GetMaxFrequencyRange(int bin_start, int bin_end);

extern bool detectSnare();

extern bool detectKick(unsigned long millisecons, unsigned long minPausBetweenCalls);

extern bool detectHiHat();


#endif