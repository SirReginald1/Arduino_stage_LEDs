//#include "Microphone.h"
#include "Animation.h"
//#include "Com_interface.h"
//#include <TimerOne.h>
#include <FastLED.h>
#include "Globals.h"
#include <Arduino.h>

#define FASTLED_INTERNAL //remove annoying pragma messages
// Animation varaibels
/*The number of LEDs per array*/
//#define NUM_LEDS 122
/*The number of arrays hooked up to the arduino*/
//#define NB_ARRAYS 4
/*The maiximum number of parameters present in an animation function*/
//#define NB_PARAMS 10
/*Indicated if the program should be run by the real time animation interface or just animation numbers*/
//#define USE_INTERFACE true
/*Indecates if the the microphone should be used*/
//#define USE_MIC true
// Setting LED pins
#define LED1_PIN 19//4
#define LED2_PIN 18//7
#define LED3_PIN 8
#define LED4_PIN 12
// Setting button pins
#define BTN_PIN 2

#ifdef USE_MIC
  const int mic_pin = A0;
  const int mic_pin2 = A1;
#endif

// Define led strips
//CRGB leds1[NUM_LEDS];
//CRGB leds2[NUM_LEDS];
//CRGB leds3[NUM_LEDS];
//CRGB leds4[NUM_LEDS];

CRGB led_arrays[NB_ARRAYS][NUM_LEDS];

// ########################## Console com variables ##########################
#ifdef USE_INTERFACE
  // The value that indicates which animation to run.
  extern int animation;
  // The index of the array currently being manipulated
  extern int current_array;
#endif

#ifndef USE_INTERFACE
  // The value that indicates which animation to run.
  int animation;
  // The index of the array currently being manipulated
  //int current_array;
  /*
  int animations[9][10] = {
    {2000, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // rainbow circle {delay}
    {255, 255, 255, -1, -1, -1, -1, -1, -1, -1}, // faide {red, green, blue}
    {0, 255, 100, 0, -1, -1, -1, -1, -1, -1}, // sparkle {red, green, blue, delay}
    {50, 50, 0, 1, -1, -1, -1, -1, -1, -1}, // fire {flame_height, sparks, delay, fire_intensity}
    {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
    {51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
    {61, 62, 63, 64, 65, 66, 67, 68, 69, 70},
    {71, 72, 73, 74, 75, 76, 77, 78, 79, 80},
    {81, 82, 83, 84, 85, 86, 87, 88, 89, 90}
  };
  */
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
  animation = 1;
  //current_array = 0;
}


void loop() {
  millisecs = millis();
  //Serial.println(millisecs);
  
  // ####################### MIC CODE #########################
  #ifdef USE_MIC
    sample_mic(mic_pin);
    sample_max_amp(millisecs);
  #endif
  
  //Serial.println(get_amplitude());
  
  // ############# LED CODE ##############
  if(Serial.available() > 0){
    /*
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
    */
    animation = Serial.parseInt();
    //animations[2][0] = Serial.parseInt();
    //Serial.print("Anim param: ");Serial.println(animations[2][0]);
    ////read_input();
    //Serial.print("Switch to: ");Serial.println(animation);
    ////serialFlush();
    FastLED.clear();
  }
  //min(1, amplitude/max_amp_in_interval)*NUM_LEDS
  //float(amplitude)/(float(max_amp_in_interval)*2)
  switch (animation) {
    case 1:
      rainbowCycle(led_arrays, 2000, millisecs);
      break;
    case 2:
      for(int i=0;i<NB_ARRAYS;i++){
        fadeAnimation(led_arrays[i], random(255), random(255), random(255));
      }
      //fadeAnimation(led_arrays[1], random(&animations[1][0]), random(&animations[1][1]), random(&animations[1][2]));
      break;
    case 3:
      Sparkle(led_arrays, 0, 255, 0, 0, millisecs);
      break;
    case 4:
      for(int i=0;i<NB_ARRAYS;i++){
        Fire(led_arrays[i], 50, 50, 0, 1);
      }
      break;
    case 5:
      for(int i=0;i<NB_ARRAYS;i++){
        shootingStarAnimation(led_arrays[i], 150, 0, 150, 20, 10, 2000, 1, millisecs);
      }
      break;
    case 6:
      TwinklePixels(led_arrays[0], 200, 50, 50, 20, 100);
      break;
    /*case 7:
      #ifdef USE_MIC
        volum_bar_animation(led_arrays[0], millisecs, NUM_LEDS);
      #endif
      #ifndef USE_MIC
        strobe(led_arrays, 20, 55, CRGB(255, 255, 255));
      #endif
      break;*/
    case 8:
      strobe(led_arrays, 20, 55, CRGB(255, 255, 255));
      break;
    case 9:
      //zip_animation(led_arrays, 2, 10, NUM_LEDS-15, 0, 20, millisecs, CRGB(0,0,255));
      break;
    default:
      //Serial.println("Animation code not recognized!");
      animation = 1;
  }
  
}
