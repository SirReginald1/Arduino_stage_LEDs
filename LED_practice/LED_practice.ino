#include <FastLED.h>

#define NUM_LEDS 50
#define LED_PIN 4

CRGB leds[NUM_LEDS];

CRGB red = CRGB::Green;
CRGB green = CRGB::Red;

int length(CRGB* array){
  return sizeof(array)/sizeof(array[0]);
}

void clear_all(){
  
}

void streak(int size, int start, int end, int speed){

  // Create a sub array
  CRGB array[size];

  for(int i=start;i<end;i++){

    // Fill array with values from section of full led array
    for(int i=0;i<size;i++){
      array[i] = leds[i+start];
    }

    delay(speed);
    FastLED.show();  
  }
}


void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(50); // Range 0 - 255
  // Make sure all lights are off
  
  for(int i=0;i<length(leds);i++){
    leds[i] =  CRGB::Black;
    FastLED.show();
  }
}

void loop() {

  //streak(5, 0, 20, 200);

  fill_gradient_RGB(leds, 5, CRGB::Green, CRGB::Blue);
  FastLED.show();
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
