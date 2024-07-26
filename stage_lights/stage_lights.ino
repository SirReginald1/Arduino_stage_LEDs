#include "Microphone.h"
#include "Animation.h"
#include <TimerOne.h>
#include <FastLED.h>

#define NUM_LEDS 122
#define LED1_PIN 4
//#define LED2_PIN 7
#define BTN_PIN 2


const int mic_pin = A0;
const int mic_pin2 = A1;

// Define led strips
CRGB leds1[NUM_LEDS];
//CRGB leds2[NUM_LEDS];
//CRGB leds3[NUM_LEDS];
//CRGB leds4[NUM_LEDS];

// ########################## Console com variables ##########################
// The value that indicates which animation to run.
int animation;
// The value that will read the input from the console at each loop.
int console_reader;

// ############################# Time variables ##############################
// Millisecond counter
unsigned long millisecs;
// Leds1 millisecond counter
unsigned long millisec_led1 = 0;/*
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

int length(CRGB* array){
  return sizeof(array)/sizeof(array[0]);
}

/**
The code to be executed during a hardware interupt.
*/
void hardware_interrupt(){
  int next_value = (animation+1) % 9;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}

/**
Toggle value for a button.
*/
volatile bool toggleState = false;
/**
Function that checks if a switch is pressed and if so starts an interupt.
*/
void interupt_switch(){
  Serial.println("Entered iterupt.");
  Serial.print("toggle value:"); Serial.println(toggleState);
  if(digitalRead(BTN_PIN) == LOW){
    toggleState = !toggleState;
  }
  //digitalWrite(LED1_PIN, !toggleState);
  hardware_interrupt();
}


/**
The code to be executed during an software interupt.
*/
void software_interrupt(){
  int next_value = (animation+1) % 7;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}

// Make zip right and zip left then function that can alternate the 2
/*
unsigned long zip_animation_prev_mills = 0;
int zip_animation_pos_counter = 0;
void zip_animation(CRGB* leds, int size, int start, int end, int delay, unsigned long current_time, CRGB color){
  int start_pos, end_pos;
  if(current_time - zip_animation_prev_mills > speed){
    // This is the start of the strip
    start_pos = zip_animation_pos_counter + start;
    if(start_pos <= NUM_LEDS && start_pos < end){
      leds[start_pos] = color;
    }

    // This is the end of the strip
    end_pos = zip_animation_pos_counter + start - size;
    if(end_pos >= 0 && end_pos <= NUM_LEDS){
      leds[end_pos] = CRGB::Black;
    }
    FastLED.show();
    zip_animation_pos_counter = ++zip_animation_pos_counter % (end - start + size);
    zip_animation_prev_mills = current_time;
  }
  
}
*/
/*
void back_and_forth(CRGB* leds, int size, int start, int end, int speed, unsigned long current_time, CRGB color){
  int start_pos, end_pos;
  switch (zip_animation_pos_counter < end) {
    case true:
      if(current_time - zip_animation_prev_mills > speed){
        // This is the start of the strip
        start_pos = zip_animation_pos_counter + start;
        if(start_pos <= NUM_LEDS && start_pos < end){
          leds[start_pos] = color;
        }

        // This is the end of the strip
        end_pos = zip_animation_pos_counter + start - size;
        if(end_pos >= 0 && end_pos <= NUM_LEDS){
          leds[end_pos] = CRGB::Black;
        }
        FastLED.show(zip_animation_pos_counter);
        if(true){
          zip_animation_pos_counter = ++zip_animation_pos_counter % (end - start + size);
        }
        zip_animation_prev_mills = current_time;
      }
      break;
    case false:
      // Run in reverse.
  }
}
*/
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

void setup() {
  // #########################################################
  // ###################### HARDWARE #########################
  // #########################################################
  // ################## PIN SETUP ####################
  // Set btn pin
  pinMode(BTN_PIN, INPUT_PULLUP);

  // following three lines to speed up ADC (that is analogRead() )
  //bitClear(ADCSRA,ADPS0);
  //bitSet(ADCSRA,ADPS1);
  //bitClear(ADCSRA,ADPS2);
  // ################ INTERUPT SETUP ##################
  // Setting hardwar interupt. Executes the interupt function when stat of pin 2 changes
  attachInterrupt(digitalPinToInterrupt(2), interupt_switch, FALLING);
  // Initializing sofware iterupt. (in microseconds)
  //TimerOne.initialize(1000000);
  // Setting software interupt
  //TimerOne.attachInterrupt(software_interrupt);
  // ################ IN OUT CONSOLE ##################
  // Setting up console output.
  Serial.begin(9600);//2000000
  // Setting the time the program will wait for input.
  Serial.setTimeout(0);
  // #########################################################
  // ######################### LED ###########################
  // #########################################################
  // Setup all the led
  FastLED.addLeds<WS2812B, LED1_PIN, RGB>(leds1, NUM_LEDS);
  //FastLED.addLeds<WS2812B, LED2_PIN, RGB>(leds2, NUM_LEDS);
  // Set power limit of LED strip to 5V, 1500mA
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  // Make sure all lights are off
  FastLED.clear();
  // #########################################################
  // ################## Set variables ########################
  // #########################################################
  // Set starting animation choice
  animation = 1;
}


void loop() {
//console_reader = Serial.parseInt();

millisecs = millis();

// ####################### MIC CODE #########################
sample_mic(mic_pin);
sample_max_amp(millisecs);

//Serial.println(get_amplitude());

// ############# LED CODE ##############
console_reader = Serial.parseInt();

if(console_reader != 0){
  animation = console_reader;
  //Serial.print("Switch to: ");Serial.println(animation);
  FastLED.clear();
}
//min(1, amplitude/max_amp_in_interval)*NUM_LEDS
//float(amplitude)/(float(max_amp_in_interval)*2)
switch (animation) {
  case 1:
    rainbowCycle(leds1, 2000, millisecs);
    break;
  case 2:
    fadeAnimation(leds1, random(255), random(255), random(255));
    break;
  case 3:
    Sparkle(leds1, 0, 255, 100, 0);
    break;
  case 4:
    Fire(leds1, 50, 50, 0, 0.01);
    break;
  case 5:
    shootingStarAnimation(leds1, 150, 0, 150, 20, 10, 2000, 1, millisecs);
    break;
  case 6:
    TwinklePixels(leds1, 200, 50, 50, 20, 100);
    break;
  case 7:
    volum_bar_animation(leds1, millisecs, NUM_LEDS);
    break;
  case 8:
    //fill_gradient_RGB(leds1, NUM_LEDS, CRGB(255,255,255), CRGB(255,255,255), CRGB(255,255,255), CRGB(255,255,255));
    fill_solid(leds1, NUM_LEDS, CRGB(255,255,255));
    FastLED.show();
    delay(20);
    FastLED.clear();
    FastLED.show();
    delay(60); // V
    //strobe(leds1, 25, CRGB(0,255,0));
    break;
  case 9:
    zip_animation(leds1, 2, 10, NUM_LEDS-15, 0, 20, millisecs, CRGB(0,0,255));
    break;
  default:
    Serial.println("Animation code not recognized!");
    animation = 1;
}
/*
if(animation == 1){
 rainbowCycle(leds1, 5);
 //clear_sequential(leds1, 0, 0);
}
else if (animation == 2) {
  //fadeAnimation(leds1, random(255), random(255), random(255));
}
else if (animation == 3) {
  Sparkle(leds1, 0, 255, 100, 0);
}
else if (animation == 4) {
  Fire(leds1, 50, 50, 0, 0.01);
}
else if (animation == 5) {
  shootingStarAnimation(leds1, 150, 0, 150, 20, 10, 2000, 1);
}
else if (animation == 6) {
  TwinklePixels(leds1, 200, 50, 50, 20, 100);
}
else{
  Serial.println("Animation code not recognized!");
  animation = 1;
}
*/
/*
  for(int i=1;i<NUM_LEDS-1;i++){
    fill_gradient_RGB(leds1, i, CRGB::Green, CRGB::Turquoise, CRGB::Magenta, CRGB::Red);
    FastLED.show();
    delay(10);
  }
  clear_all();
*/
/*
  int nb = 20;
  //streak(5, 0, 20, 200);
  for(int i=1;i<nb;i++){
    for(int j=i;j<nb;j++){
      leds[j] =  CRGB::Black;
    }
    fill_gradient_RGB(leds, i, CRGB::Green, CRGB::Blue);
    FastLED.show();
    delay(50);
  }*/
  /*
  int i;
  for(i=0;i<20;i++){
    leds[i-1] = CRGB::Black;
    leds[i] = red;
    leds[i+1] = green;
    leds[i+2] = CRGB::Blue;
    

    FastLED.show();
    delay(50);

    //if(i>0){
    //  leds[i-1] = CRGB::Black;
    //  FastSPI_LED.show();
    //}
  }
  
    leds[i-1] = CRGB::Black;
    leds[i] = CRGB::Black;
    leds[i+1] = CRGB::Black;
    //leds[i+2] = CRGB::Black;
    FastSPI_LED.show();
    */
  
}
