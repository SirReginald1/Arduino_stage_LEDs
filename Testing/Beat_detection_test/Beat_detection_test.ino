#include <Arduino.h>
#include <driver/i2s.h>
#include <arduinoFFT.h>
#include "Microphone.h"
#include "Beat_detector.h"


void setup(){
  Serial.begin(115200);
  beatDetectionSetup();
}

void loop() {
  getFFT();
  //Serial.println(vReal[0]);
  //for (int i = 0; i < 1024; i++) {
  //  Serial.println(vReal[i]);
  //}

  //Serial.print("Snaire: ");Serial.println(GetMaxFrequencyRange(200,220));
  //Serial.print("Kick: ");Serial.println(GetMaxFrequencyRange(80,180));
  //Serial.print("HiHat: ");Serial.println(GetMaxFrequencyRange(300,3000));

  // Analyze frequency bins for sound detection and print result
  
  //if (detectSnare()) {
  //  Serial.println("Snare detected!");
  //}
  
  if (detectKick(millis(), 5)) {
    Serial.println("Kick detected!");
  }
  
  //if (detectHiHat()) {
  //  Serial.println("Hi-hat detected!");
  //}
 
}
/*
// Function to detect snare sound by analyzing FFT frequency bins
bool detectSnare() {
  // Snare drum typically has energy in 150 Hz to 400 Hz
  int bin_start = frequencyToBin(150);
  int bin_end = frequencyToBin(400);
  float threshold = 5000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}

// Function to detect kick drum sound
bool detectKick() {
  // Kick drum typically has energy in 40 Hz to 100 Hz
  int bin_start = frequencyToBin(40);
  int bin_end = frequencyToBin(100);
  float threshold = 7500;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}

// Function to detect hi-hat sound
bool detectHiHat() {
  // Hi-hats typically have energy in 5 kHz to 10 kHz
  int bin_start = frequencyToBin(5000);
  int bin_end = frequencyToBin(10000);
  float threshold = 40000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
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
}*/