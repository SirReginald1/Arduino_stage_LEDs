#ifndef Animations_h
#define Animations_h

#include <FastLED.h>
#include <Arduino.h>
#include "Globals.h"
#include "Params.h"

/**Struct that contains the number of the animation being run by the array it's attributed to
 *  as well as all the references to the aniamtion parameters.
 * */
struct animParamRef{

  /** This variable is used to idetify the array when using multi-array animations  */
  u_int16_t arrayId;

  int nbLeds = NUM_LEDS; // The number of leds in the assigned array

  int animation = 3; // The animation the array is currentlly set to

  int animationPosition[NB_ANIMATIONS]; // The array containing the animation position for each of the animations.

  int rainbowCycleParamInt[1] = {0}; // {int delay}
  //int rainbowCycleVarInt[1] = {0}; // {point in animation}
  unsigned long rainbowCycleLastActivate = 0;

  int fadeInAndOutParamInt[4] = {255, 255, 255, 100}; // {int red, int green, int blue, int speed}
  //int fadeInAndOutFadeAmount = 0;
  bool fadeInAndOutFadingIn = true;
  unsigned long fadeInAndOutLastActivate = 0;

  int sparkleParamInt[5] = {200, 0, 100, 0}; // {int red, int green, int blue, int delay}
  int sparkleVarInt[1] = {0}; // {int lastPixel}

  int fireParamInt[3] = {50, 50, 25}; // {int flame_height, int sparks, int delay}
  float fireParamFloat[1] = {1.}; // {float fire_intensity}
  byte* fireHeat = new byte[nbLeds];
  unsigned long fireLastActivate = 0;

  int shootingStarParamInt[7] = {150, 0, 150, 20, 10, 2000, 1}; // {int R, int G, int B, int tail_length, int delay_duration, int interval, int direction}
  unsigned long shootingStarLastActivate = 0;           // Stores last time LEDs were updated
  unsigned long shootingStarLastStar = 0; // The last time a shooting star started
  //int shootingStarPosCounter = 0;

  int twinklePixelsParamInt[5] = {200, 50, 50, 100, 50}; // {int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration}
  unsigned long twinklePixelsLastActivate = 0;

  int strobeParamInt[5] = {20, 55, 255, 255, 255}; // {int time_on, int time_off, int R, int G, int B}
  unsigned long strobeLastOn = 0;
  unsigned long strobeLastOff = 0;

  int zipParamInt[7] = {2, 10, NUM_LEDS-5, 0, 0, 0, 255}; // {int size, int start, int end, int delay, int R, int G, int B}
  unsigned long zipParamUnsignedLong[1] = {20}; // {unsigned long speed, unsigned long current_time}
  //int zipPosCounter = 0;
  unsigned long zipLastActivate = 0;

  int flashToBeatParamInt[4] = {255,255,255,50}; // {red, green, blue, time_left_on}
  bool flashToBeatIsOn = false;
  unsigned long flashToBeatLastActivate = 0;
};

animParamRef newAnimParamRef(int nbLeds);

class Animations{
  public:

// #########################################################################
// ############################## Class variables ##########################
// #########################################################################

    /** Indicates if the flashToBeat animation should start. */
    static bool flashToBeatGo;

    /** Indicates if the flashToBeat animation has started. */
    static bool flashToBeatStarted;

    static void stopFlashToBeatArray();

    //static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefArray[NB_ARRAYS], unsigned long millisecs);

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], StackAnimParamRef (&animParamRefArray)[NB_ARRAYS]);

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
};

#endif