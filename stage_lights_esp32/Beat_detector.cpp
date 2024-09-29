#include "esp32-hal.h"
#include "freertos/queue.h"
#include <arduinoFFT.h>
#include "Audio.h"
#include "Beat_detector.h"
#include "Globals.h"

extern QueueHandle_t core1NotifQueue;

const uint_fast16_t samples = NB_SAMPLES_ADC; //This value MUST ALWAYS be a power of 2
////const float signalFrequency = 1000;
const double samplingFrequency = SAMPLING_FREQUENCY_ADC;

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
float vReal[NB_SAMPLES_MIC] = {0};
/** The array of imaginary numbers used in the FFT. */
float vImag[NB_SAMPLES_MIC] = {0};
/** The array buffer used to read the microphone values. */
int buffer[NB_SAMPLES_MIC] = {0};
/** The number of bites writen to the buffer variable affter a call to the I2C read function. */
//size_t bytesIn = 0;

/** The FFT object used to perform FFT transformations. */
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency); /* Create FFT object */

void beatDetectionSetup(){
  Audio::setup();
}

//void switchAudioInputOff(){
//  Serial.println("Audio input off");
//  Audio::switchPortsOff();
//}

/**
 * The function switches the FFT object with one that posseses the samples and samplingFrequency appropriate to the periferal.
 * 
 * @param periferalCode The code that represents the periferal. Options are {PERIF_CODE_ADC, PERIF_CODE_MIC}
 */
void switchFFT(uint8_t periferalCode) {
  uint_fast16_t newSamples;
  double newSamplingFrequency;
  switch (periferalCode){
    case MODE_ADC_FFT_ON:
      Audio::switchAudioInput(MODE_ADC_FFT_ON);
      Serial.printf("After switch to adc port nb %d",Audio::currentPort);
      newSamples = NB_SAMPLES_ADC;
      newSamplingFrequency = SAMPLING_FREQUENCY_ADC;
      FFT = ArduinoFFT<float>(vReal, vImag, newSamples, newSamplingFrequency);
      break;
    case MODE_MIC_FFT_ON:
      Audio::switchAudioInput(MODE_MIC_FFT_ON);
      Serial.printf("After switch to mic port nb %d",Audio::currentPort);
      newSamples = NB_SAMPLES_MIC;
      newSamplingFrequency = SAMPLING_FREQUENCY_MIC;
      FFT = ArduinoFFT<float>(vReal, vImag, newSamples, newSamplingFrequency);
      break;
    case MODE_FFT_OFF:
      Audio::switchAudioInput(MODE_FFT_OFF);
      break;
    default:
      break;
  }
}

float low_pass_filter(int current_value, float previous_value, float alpha) {
    return alpha * (float)current_value + (1 - alpha) * previous_value;
}

float filter_adc_value(int adc_value) {
    static float filtered_value = 0;
    float alpha = 0.1;  // Smoothing factor (between 0 and 1)
    filtered_value = low_pass_filter(adc_value, filtered_value, alpha);
    //Serial.printf("adcv: %f\n",adc_value);
    return filtered_value;
}

void getFFT() {
  Audio::readAudio<int>(buffer);
  for (int i = 0; i < samples; i++) {
    //vReal[i] = filter_adc_value(buffer[i]);  // Copy microphone samples into the real part of FFT input
    vReal[i] = ((float)buffer[i])/((float)INT_MAX);
    //Serial.printf(">b: %d\n",buffer[i]);
    //Serial.printf(">v: %f\n",vReal[i]);
    vImag[i] = 0.0;  // Imaginary part is zero for FFT input
    //Serial.println(vReal[i]);
  }
  //for(int i=0;i<samples;i++){
  //  Serial.println(vReal[i]);
  //}
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
  //for(int i=0;i<samples;i++){
  //  Serial.printf(">f: %f\n", vReal[i]);
  //}
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
    Serial.println(vReal[i]);
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

/** 
  * Detects frequency in the stated band is greater than intensity.
  * 
  * @param band Array containing lower limit, upper limit, and intensity threshold.
  * @param minPausBetweenCalls The minimum amount of time to wait between calls.
  * @param lastCall Time sinse last call for that bandwidth.
*/
bool detectBand(double band[2], float intensity, unsigned long minPausBetweenCalls, unsigned long &lastCall) {
  unsigned long current_time = millis();
  if((lastCall - current_time) < minPausBetweenCalls){
    return false;
  }
  lastCall = current_time;
  //Serial.printf("low: %d, high: %d, int: %f\n",band[0], intensity, band[1]);
  //Serial.println(band[0]);
  //Serial.println(band[1]);
  // Kick drum typically has energy in 40 Hz to 100 Hz. The magic values seem to be 80 to 180 Hz.
  return checkFrequencyRange(frequencyToBin(band[0]), frequencyToBin(band[1]), intensity); // Adjust based on experimentation
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
bool detectKick(unsigned long minPausBetweenCalls) {
  static unsigned long lastCall;
  unsigned long current_time = millis();
  if((lastCall - current_time) < minPausBetweenCalls){
    return false;
  }
  lastCall = current_time;
  // Kick drum typically has energy in 40 Hz to 100 Hz. The magic values seem to be 80 to 180 Hz.
  return checkFrequencyRange(frequencyToBin(80), frequencyToBin(180), 5000000000); // Adjust based on experimentation
}

// Function to detect hi-hat sound
bool detectHiHat() {
  // Hi-hats typically have energy in 5 kHz to 10 kHz
  int bin_start = frequencyToBin(300);
  int bin_end = frequencyToBin(3000);
  float threshold = 5000000000;  // Adjust based on experimentation
  
  return checkFrequencyRange(bin_start, bin_end, threshold);
}