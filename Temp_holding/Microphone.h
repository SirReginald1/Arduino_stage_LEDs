#ifndef Microphone_h
#define Microphone_h

#include "Globals.h"
#include <FastLED.h>
#include <Arduino.h>

/*
// Baseline amplitude
const int basline_amp;
// Value picked up directly by the microphone
int mic_amp;
// The sound picked up by the mick minus the baseline background noise
float amplitude;
// The maximum sound amplitude detected in the last interval
float max_amp_in_interval;
// The interval of time between max amplitude samplings
const unsigned long max_amp_millis_interval;
// The previouse millisecond value for the max amplitude sampling
unsigned long max_amp_prev_mills;
*/
void sample_mic(int mic_pin);

void sample_max_amp(unsigned long millisecs);
/*
int volum_bar_last_nb;
int volum_bar_nb_led;

unsigned long volum_bar_mill;
*/
void volum_bar_animation(CRGB *leds, unsigned long millisecs, int nb_leds);

#endif