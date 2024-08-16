//#include "Microphone.h"
#include "Animations.h"
#include "Com_interface.h"
//#include <TimerOne.h>
#include <FastLED.h>
#include "Globals.h"
#include <Arduino.h>

/*Indicated if the program should be run by the real time animation interface or just animation numbers*/
//#define USE_INTERFACE
/*Indecates if the the microphone should be used*/
//#define USE_MIC

// Setting LED pins
#define LED1_PIN 4//4
#define LED2_PIN 7//7
#define LED3_PIN 8
#define LED4_PIN 12
// Setting button pins
#define BTN_PIN 2

#ifdef USE_MIC
  const int mic_pin = A0;
  const int mic_pin2 = A1;
#endif

// Define LED arrays
CRGB led_arrays[NB_ARRAYS][NUM_LEDS];

// ########################## Console com variables ##########################
/*
#ifdef USE_INTERFACE
  // The value that indicates which animation to run.
  //extern int animation;
  // The index of the array currently being manipulated
  extern int current_array;
#endif
*/
#ifndef USE_INTERFACE
  // The value that indicates which animation to run.
  int animation;
  // The index of the array currently being manipulated
  //int current_array;
#endif




// List of parameters
//int parameters[NB_PARAMS];
//extern int parameters[NB_PARAMS];
// The current number of parameters in parameters array
//int param_count;
//extern int *param_count;

// ############################# Time variables ##############################
// Millisecond counter
unsigned long millisecs;
// Leds1 millisecond counter
/*
//unsigned long millisec_led1 = 0;
// Leds2 millisecond counter
unsigned long millisec_led2 = 0;
// Leds3 millisecond counter
unsigned long millisec_led3 = 0;
// Leds4 millisecond counter
unsigned long millisec_led4 = 0;
*/

/*
// Pointer to milliseconds leds 1
unsigned long *millisec_led1_pnt = &millisec_led1
// Pointer to milliseconds leds 2
unsigned long *millisec_led2_pnt = &millisec_led1
// Pointer to milliseconds leds 3
unsigned long *millisec_led3_pnt = &millisec_led1
// Pointer to milliseconds leds 4
unsigned long *millisec_led4_pnt = &millisec_led1
*/

/**
The code to be executed during a hardware interupt.
*/
/*
void hardware_interrupt(){
  int next_value = (animation+1) % 9;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}
*/
/**
Toggle value for a button.
*/
//volatile bool toggleState = false;
/**
Function that checks if a switch is pressed and if so starts an interupt.
*/
/*
void interupt_switch(){
  Serial.println("Entered iterupt.");
  Serial.print("toggle value:"); Serial.println(toggleState);
  if(digitalRead(BTN_PIN) == LOW){
    toggleState = !toggleState;
  }
  //digitalWrite(LED1_PIN, !toggleState);
  hardware_interrupt();
}
*/

/**
The code to be executed during an software interupt.
*/
/*
void software_interrupt(){
  int next_value = (animation+1) % 7;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}
*/
/*
void rain_animation(){
  // Used zip on random start (end = start - size) on all LED bars
}

void wrestle(){
 // ####################////////////////////////////
 // ######################/////////////////////////
 // #################//////////////////////////////
}


void disolve_to_black(CRGB* leds, int speed){


}

*/
void setup() {
  // #########################################################
  // ###################### HARDWARE #########################
  // #########################################################
  // ################## PIN SETUP ####################
  // Set btn pin
  //pinMode(BTN_PIN, INPUT_PULLUP);

  // following three lines to speed up ADC (that is analogRead() )
  //bitClear(ADCSRA,ADPS0);
  //bitSet(ADCSRA,ADPS1);
  //bitClear(ADCSRA,ADPS2);
  // ################ INTERUPT SETUP ##################
  // Setting hardwar interupt. Executes the interupt function when stat of pin 2 changes
  //attachInterrupt(digitalPinToInterrupt(2), interupt_switch, FALLING);
  // Initializing sofware iterupt. (in microseconds)
  //TimerOne.initialize(1000000);
  // Setting software interupt
  //TimerOne.attachInterrupt(software_interrupt);
  // ################ IN OUT CONSOLE ##################
  // Setting up console output.
  Serial.begin(115200);//9600
  // Setting the time the program will wait for input.
  Serial.setTimeout(100);
  // #########################################################
  // ######################### LED ###########################
  // #########################################################
  // Setup all the led
  //for(int i=0;i<NUM_LEDS;i++){
  //}
  FastLED.addLeds<WS2812B, LED1_PIN, RGB>(led_arrays[0], NUM_LEDS);
  //FastLED.addLeds<WS2812B, LED2_PIN, RGB>(led_arrays[1], NUM_LEDS);
  //FastLED.addLeds<WS2812B, LED3_PIN, RGB>(led_arrays[2], NUM_LEDS);
  //FastLED.addLeds<WS2812B, LED4_PIN, RGB>(led_arrays[3], NUM_LEDS);
  // Set power limit of LED strip to 5V, 1500mA
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  // Make sure all lights are off
  FastLED.clear();
  // #########################################################
  // ################## Set variables ########################
  // #########################################################
  // Set starting animation choice
  #ifndef USE_INTERFACE
  animation = 1;
  //current_array = 0;
  #endif
  #ifdef USE_INTERFACE
  ComInterface::setAnimation(1);
  #endif
}


void loop() {
  millisecs = millis();
  
  // ####################### MIC CODE #########################
  #ifdef USE_MIC
    sample_mic(mic_pin);
    sample_max_amp(millisecs);
  #endif
  
  //Serial.println(get_amplitude());
  
  // ############# LED CODE ##############
  if(Serial.available() > 0){

    #ifdef USE_INTERFACE
    ComInterface::readInput();
    #endif

    #ifndef USE_INTERFACE
    animation = Serial.parseInt();
    #endif

    FastLED.clear();
  }

  Animations::runAnimations(led_arrays, millisecs);
  /*
  #ifdef USE_INTERFACE
  switch (ComInterface::getAnimation()) {
  #endif
  #ifndef USE_INTERFACE
  switch (animation) {
  #endif
    case 1:
      Animations::rainbowCycle(led_arrays, 2000, millisecs);
      //Test::test();
      break;
    case 2:
      for(int i=0;i<NB_ARRAYS;i++){
        Animations::fadeInAndOut(led_arrays[i], random(255), random(255), random(255));
      }
      //Animations::fadeInAndOut(led_arrays[1], random(&animations[1][0]), random(&animations[1][1]), random(&animations[1][2]));
      break;
    case 3:
      Animations::sparkle(led_arrays, 255, 100, 100, 0, millisecs);
      break;
    case 4:
      for(int i=0;i<NB_ARRAYS;i++){
        Animations::Fire(led_arrays[i], 50, 50, 0, 1.);
      }
      break;
    case 5:
      for(int i=0;i<NB_ARRAYS;i++){
        Animations::shootingStar(led_arrays[i], 150, 0, 150, 20, 10, 2000, 1, millisecs);
      }
      break;
    case 6:
      Animations::twinklePixels(led_arrays[0], 200, 50, 50, 20, 100);
      break;
    /*case 7:
      #ifdef USE_MIC
        volum_bar_animation(led_arrays[0], millisecs, NUM_LEDS);
      #endif
      #ifndef USE_MIC
        Animations::strobe(led_arrays, 20, 55, CRGB(255, 255, 255));
      #endif
      break;*//*
    case 8:
      Animations::strobe(led_arrays, 20, 55, CRGB(255, 255, 255));
      break;
    case 9:
      Animations::zip(led_arrays, 2, 10, NUM_LEDS-15, 0, 20, millisecs, CRGB(0,0,255));
      break;
    default:
      Serial.println("Animation code not recognized!");

      #ifndef USE_INTERFACE
      animation = 1;
      #endif

      #ifdef USE_INTERFACE
      ComInterface::setAnimation(1);
      #endif
  }
  */
}
