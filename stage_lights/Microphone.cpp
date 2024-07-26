#include "Microphone.h"

#define NUM_LEDS 122
//extern unsigned long millisecs;

/*
class Microphone {
  public:
*/

// ################### MICROPHONE VARIABLES ######################
// The basline backgound noise
//int basline_amp = 200;
// Value picked up directly by the microphone
int mic_amp;
// The sound picked up by the mick minus the baseline background noise
float amplitude;
// The maximum sound amplitude detected in the last interval
float max_amp_in_interval = 0.01;
// The interval of time between max amplitude samplings
const unsigned long max_amp_millis_interval = 200;
// The previouse millisecond value for the max amplitude sampling
unsigned long max_amp_prev_mills = 0;

void sample_mic(int mic_pin){
  mic_amp = abs(analogRead(mic_pin));

  amplitude = max(mic_amp - 200/*basline_amp*/, 0);
}

float get_amplitude(){
  return amplitude;
}

/*
  * Samples the amplitude at specified intervals
*/
void sample_max_amp(unsigned long millisecs){
  if(millisecs - max_amp_prev_mills > max_amp_millis_interval){
    max_amp_in_interval = amplitude;
    max_amp_prev_mills = millisecs;
  }
}


 int volum_bar_last_nb = 0;
  int volum_bar_nb_led = 0;
  unsigned long volum_bar_mill = 0;
  void volum_bar_animation(CRGB *leds, unsigned long millisecs, int nb_leds){
    volum_bar_nb_led = min((amplitude)/max_amp_in_interval, nb_leds);//(amplitude/max_amp_in_interval > 0.01 ?  amplitude/max_amp_in_interval : 0.01) *NUM_LEDS;
    //nb_led = random(0,NUM_LEDS);
    // Fill loop
    if(millisecs - volum_bar_mill > 2000){
      volum_bar_mill = volum_bar_mill;
    for(int i=0; i<volum_bar_nb_led; i++){
      leds[i] = CRGB(random(0,50),random(0.150),random(0,50));
    }
    for(int i=volum_bar_nb_led; i<NUM_LEDS; i++){
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    }
  }

//};