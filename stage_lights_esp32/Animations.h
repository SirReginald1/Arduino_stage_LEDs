#ifndef Animations_h
#define Animations_h

#include <FastLED.h>
#include <Arduino.h>
#include "Globals.h"

struct animParamRef{
  int rainbowCycleParamInt[1] = {2000};

  int fadeInAndOutParamInt[3] = {255, 255, 255};

  int sparkleParamInt[4] = {255, 255, 255, 0};

  int fireParamInt[3] = {50, 50, 0};
  float fireParamFloat[1] = {1.};

  int shootingStarParamInt[7] = {150, 0, 150, 20, 10, 2000, 1};

  int twinklePixelsParamInt[5] = {200, 50, 50, 20, 100};

  int strobeParamInt[5] = {20, 55, 255, 255, 255};

  int zipParamInt[7] = {2, 10, NUM_LEDS-5, 0, 0, 0, 255};
  unsigned long zipParamUnsignedLong[1] = {20};
};

class Animations{
  public:

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefArray[NB_ARRAYS], unsigned long millisecs);

    static int* getParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    static float* getParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    static unsigned long* getParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    /**
      * Returns the array of integer parameters for specified animation.
      *
      * @param animationCode The code for the animation the integer parameters of which are to be retreved.
      * @param animationStruct The struct that references the integer parameter values for a particular LED array.
      * @return The array of integer parameters for the specified animation in order of specification in function signiature.
    */
    static void setParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, int paramValue);

    static void setParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, float paramValue);

    static void setParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, unsigned long paramValue);

    static void rainbowCycle(CRGB* leds, int DelayDuration, int millisecs);
    static void rainbowCycle(CRGB leds[NB_ARRAYS][NUM_LEDS], int DelayDuration, int millisecs);

    static void fadeInAndOut(CRGB* leds, int red, int green, int blue);

    static void sparkle(CRGB* leds, int red, int green, int blue, int delayDuration, unsigned long millisecs);
    static void sparkle(CRGB leds[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefs[NB_ARRAYS], unsigned long millisecs);

    /*
      * FlameHeight - Use larger value for shorter flames, default=50.
      * Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
      * DelayDuration - Use larger value for slower flame speed, default=10.
    */
    static void Fire(CRGB* leds, int FlameHeight, int Sparks, int DelayDuration, float intensity);

    /* 
     *  ========== Shooting Star Animation ==========
     *  red, green, blue - Choose a color with RGB values (0 to 255).
     *  @param tail_length - A larger value results in shorter tail.
     *  @param delay_duration - A larger value results in slower movement speed.
     *  @param interval - Time interval between new shooting stars (in milliseconds).
    */
    static void shootingStar(CRGB* leds, int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction, unsigned long currentMillis);

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
    static void twinklePixels(CRGB* leds, int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration);

    static void zip(CRGB* leds, int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);
    static void zip(CRGB leds[NB_ARRAYS][NUM_LEDS], int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);

    static void strobe(CRGB* leds, int time_on, int time_off, CRGB color);
    static void strobe(CRGB leds[NB_ARRAYS][NUM_LEDS], int time_on, int time_off, CRGB color);

    /*
      * Clears the whole strip sequentialy in the specified direction.
      * 
      * @param leds The array of leds on which to opperate.
      * @param direction If 0 will run from plug to end. Else will run from top to plug.
      * @param speed Length of the delay in millisecs between each led turning off.
    */
    static void clear_sequential(CRGB* leds, int direction, int speed);
};

#endif