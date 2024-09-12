#include "Microphone.h"
#include <arduinoFFT.h>
#include"Beat_detector.h"


const uint_fast16_t samples = NB_SAMPLES; //This value MUST ALWAYS be a power of 2
////const float signalFrequency = 1000;
const double samplingFrequency = 100000;
////const uint8_t amplitude = 100;

/** The array of real numbers used in the FFT. */
float vReal[NB_SAMPLES] = {0};
/** The array of imaginary numbers used in the FFT. */
float vImag[samples] = {0};
/** The array buffer used to read the microphone values. */
long buffer[samples] = {0};
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
  float threshold = 5000000000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}

/** 
* Detects if a kick is present in the music
*/
bool detectKick(unsigned long millisecons, unsigned long minPausBetweenCalls) {
  static unsigned long lastCall;
  if((lastCall - millisecons) < minPausBetweenCalls){
    lastCall = millisecons;
    return false;
  }
  // Kick drum typically has energy in 40 Hz to 100 Hz
  int bin_start = frequencyToBin(80);
  int bin_end = frequencyToBin(180);
  float threshold = 5000000000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}

// Function to detect hi-hat sound
bool detectHiHat() {
  // Hi-hats typically have energy in 5 kHz to 10 kHz
  int bin_start = frequencyToBin(300);
  int bin_end = frequencyToBin(3000);
  float threshold = 5000000000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}