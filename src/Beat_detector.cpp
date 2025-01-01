#include "esp32-hal.h"
#include "freertos/queue.h"
#include <arduinoFFT.h>
#include "Microphone.h"
#include "Beat_detector.h"
#include "Globals.h"

extern QueueHandle_t core1NotifQueue;

const uint_fast16_t samples = NB_SAMPLES; //This value MUST ALWAYS be a power of 2
////const float signalFrequency = 1000;
const double samplingFrequency = SAMPLING_FREQUENCY;
////const uint8_t amplitude = 100;
/** Value indicating what audio feature hase been detected. If none detected will be set to NO_AUDIO_FEATURE_DETECTED. */
/*volatile*/u_int16_t FFTAudioFeatureDetected = NO_AUDIO_FEATURE_DETECTED;
/** Indicates if a kick is detected */
bool detectedKick = false;
/** Indicates if a snaire is detected */
bool detectedSnaire = false;
/** Indicates if a high hat is detected */
bool detectedHighHat = false;

/** The array of real numbers used in the FFT. */
float vReal[NB_SAMPLES] = {0};
/** The array of imaginary numbers used in the FFT. */
float vImag[NB_SAMPLES] = {0};
/** The array buffer used to read the microphone values. */
long buffer[NB_SAMPLES] = {0};
/** The number of bites writen to the buffer variable affter a call to the I2C read function. */
//size_t bytesIn = 0;

/** The FFT object used to perform FFT transformations. */
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency, false); /* Create FFT object */

void beatDetectionSetup(){
  Microphone::setup(samplingFrequency);
}

void getFFT() {
  Microphone::readMic<long>(buffer);
  for (int i = 0; i < samples; i++) {
    vReal[i] = (float)buffer[i];  // Copy microphone samples into the real part of FFT input
    vImag[i] = 0.0;  // Imaginary part is zero for FFT input
    //Serial.println(vReal[i]);
  }
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
}

/**
  * Function called before runing animations to read the contense of the core1NotifQueue and set the beat detection animations indicator variables accordingly.
*/
void readBeatQueue(){
  while(xQueueReceive(core1NotifQueue, &FFTAudioFeatureDetected, (TickType_t) 0) == pdTRUE){
    switch (FFTAudioFeatureDetected) {
      case NO_AUDIO_FEATURE_DETECTED:
        return;
        break;
      case KICK_DETECTED:
        detectedKick = true;
        break;
      case SNAIRE_DETECTED:
        detectedSnaire = true;
        break;
      case HIGH_HAT_DETECTED:
        detectedHighHat = true;
        break;
      default:
        FFTAudioFeatureDetected = NO_AUDIO_FEATURE_DETECTED;
        return;
    }
    if(!detectedKick && !detectedSnaire && !detectedHighHat){
      FFTAudioFeatureDetected = NO_AUDIO_FEATURE_DETECTED;
      return;
    }
  }
  FFTAudioFeatureDetected = NO_AUDIO_FEATURE_DETECTED;
}

/**
  * Called after runing animations to reset beat detection indicators.
*/
void resetBeatDetectionVariables(){
  detectedKick = false;
  detectedSnaire = false;
  detectedHighHat = false;
}
 
// Helper function to convert frequency to bin number
int frequencyToBin(double frequency) {
  // Bin width is (Sampling Frequency) / (Number of Samples)
  double binWidth = (double)samplingFrequency / (double)samples;
  return (int)(frequency / binWidth);
}

// Helper function to check frequency range in the FFT output
bool checkFrequencyRange(int bin_start, int bin_end, float threshold) {
  for (int i = bin_start; i <= bin_end; i++) {
    if (vReal[i] > threshold) {
      return true;  // Detected
    }
  }
  return false;  // Not detected
}

// Helper function returns the maximum frequency in interval
float GetMaxFrequencyRange(int bin_start, int bin_end) {
  float freqMax = 0;
  for (int i = bin_start; i <= bin_end; i++) {
    if (vReal[i] > freqMax) {
      freqMax = vReal[i];  // Detected
    }
  }
  return freqMax;  // Not detected
}

// Function to detect snare sound by analyzing FFT frequency bins
bool detectSnare() {
  // Snare drum typically has energy in 150 Hz to 400 Hz
  int bin_start = frequencyToBin(200);
  int bin_end = frequencyToBin(220);
  float threshold = 500000000000000;  // Adjust based on experimentation 5000000000
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}

/** 
* Detects if a kick is present in the music
*/
bool detectKick(unsigned long minPausBetweenCalls) {
  static unsigned long lastCall;
  unsigned long current_time = millis();
  if((lastCall - current_time) < minPausBetweenCalls){
    return false;
  }
  lastCall = current_time;
  // Kick drum typically has energy in 40 Hz to 100 Hz. The magic values seem to be 80 to 180 Hz.
  return checkFrequencyRange(frequencyToBin(80), frequencyToBin(180), 5000000000); // Adjust based on experimentation 80, 180, 5000000000 | 120, 180, 17000000000
}

// Function to detect hi-hat sound
bool detectHiHat() {
  // Hi-hats typically have energy in 5 kHz to 10 kHz
  int bin_start = frequencyToBin(300);
  int bin_end = frequencyToBin(3000);
  float threshold = 5000000000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}