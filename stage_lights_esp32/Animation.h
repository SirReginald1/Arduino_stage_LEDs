#ifndef Animation_h
#define Animation_h

#include <FastLED.h>
#include <Arduino.h>
#include "Globals.h"

//#define NUM_LEDS 122
//#define NB_ARRAYS 4
//#define MAX_NB_PARAM 10

//class Animation {
  //public:
  /*
  * FlameHeight - Use larger value for shorter flames, default=50.
  * Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
  * DelayDuration - Use larger value for slower flame speed, default=10.
  */
  void Fire(CRGB* leds, int FlameHeight, int Sparks, int DelayDuration, float intensity);

  void setPixelHeatColor(CRGB* leds, int Pixel, byte temperature, float intensity);


  /*Stores last time LEDs were updated */
  //unsigned long shootingStarPreviousMillis;

  /**Stores count for incrementing up to the NUM_LEDs*/
  //int shootingStarCount; 

  /* 
     *  ========== Shooting Star Animation ==========
     *  red, green, blue - Choose a color with RGB values (0 to 255).
     *  @param tail_length - A larger value results in shorter tail.
     *  @param delay_duration - A larger value results in slower movement speed.
     *  @param interval - Time interval between new shooting stars (in milliseconds).
    */
  void shootingStarAnimation(CRGB* leds, int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction, unsigned long currentMillis);

  /*
      *  ========== Twinkle_Pixels_Animation ==========
      *  PARAMETERS:
      *   leds - The array of leds to make twinkle
      *   Color - select the color of the animation with one of the following numbers (or anything in between) [range = 0 to 255; default = random(256)]:
            Red: 0
            Orange: 30
            Yellow: 42
            Green: 85
            Cyan: 127
            Blue: 170
            Purple: 210
            Pink: 218
      *   ColorSaturation - Sets the color saturation according to HSV color code. Set to 0 for white and set to 255 for max saturation
      *                     [max range = 0 to 255; default = 255]
      *   PixelVolume - Use a larger value for less LEDs to twinkle [default = 20]
      *   FadeAmount - Use a larger value for a faster fade amount [max range = 0 to 255; default = 50]
      *   DelayDuration - Use a larger value for a slower animation speed [default = 50]
      *  
      *  NOTES:
      *  - You can combine twinkling colors by putting two of the function calls together in the loop() function. Try it out!
      *  - You can get many unique effects by changing the various arguments. Here are some examples to try:
      *      TwinklePixels(170, 255, 70, 20, 50);   // Blue color with lower pixel volume and slower speed
      *      TwinklePixels(0, 255, 40, 80, 80);     // Red color with moderate pixel volume and moderate speed
      *      TwinklePixels(85, 200, 15, 50, 30);    // Green color with high pixel volume and faster speed
      *      TwinklePixels(255, 0, 100, 120, 0);    // White color with low pixel volume and max speed
    */
  void TwinklePixels(CRGB* leds, int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration);

  /*
  * Clears the whole strip sequentialy in the specified direction.
  * 
  * @param leds The array of leds on which to opperate.
  * @param direction If 0 will run from plug to end. Else will run from top to plug.
  * @param speed Length of the delay in millisecs between each led turning off.
  */
  void clear_sequential(CRGB* leds, int direction, int speed);

  void strobe(CRGB* leds, int time_on, int time_off, CRGB color);
  void strobe(CRGB leds[NB_ARRAYS][NUM_LEDS], int time_on, int time_off, CRGB color);

  void Sparkle(CRGB* leds, int red, int green, int blue, int delayDuration);
  void Sparkle(CRGB leds[NB_ARRAYS][NUM_LEDS], int red, int green, int blue, int delayDuration, unsigned long millisecs);

  void fadeAnimation(CRGB* leds, int red, int green, int blue);

  void rainbowCycle(CRGB* leds, int DelayDuration, int millisecs);
  void rainbowCycle(CRGB leds[NB_ARRAYS][NUM_LEDS], int DelayDuration, int millisecs);

  void zip_animation(CRGB* leds, int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);
  void zip_animation(CRGB leds[NB_ARRAYS][NUM_LEDS], int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);
//}
#endif