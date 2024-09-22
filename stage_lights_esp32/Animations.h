#ifndef Animations_h
#define Animations_h

#include <FastLED.h>
#include <Arduino.h>
#include "Globals.h"

struct animParamRef{

  u_int16_t arrayId;

  int nbLeds = NUM_LEDS; // The number of leds in the assigned array

  int animation = 0; // The animation the array is currentlly set to

  int rainbowCycleParamInt[1] = {0};
  int rainbowCycleVarInt[1] = {0};
  unsigned long rainbowCycleLastActivate = 0;

  int fadeInAndOutParamInt[4] = {255, 255, 255, 100};
  int fadeInAndOutFadeAmount = 0;
  bool fadeInAndOutFadingIn = true;
  unsigned long fadeInAndOutLastActivate = 0;

  int sparkleParamInt[5] = {200, 0, 100, 0, 0};
  int sparkleVarInt[1] = {0}; // {int lastPixel}

  int fireParamInt[3] = {50, 50, 25};
  float fireParamFloat[1] = {1.};
  byte* fireHeat = new byte[nbLeds];
  unsigned long fireLastActivate = 0;

  int shootingStarParamInt[7] = {150, 0, 150, 20, 10, 2000, 1};
  unsigned long shootingStarLastActivate = 0;
  unsigned long shootingStarLastStar = 0; // The last time a shooting star started
  int shootingStarPosCounter = 0;

  int twinklePixelsParamInt[5] = {200, 50, 50, 100, 50};
  unsigned long twinklePixelsLastActivate = 0;

  int strobeParamInt[5] = {20, 55, 255, 255, 255};
  unsigned long strobeLastOn = 0;
  unsigned long strobeLastOff = 0;

  int zipParamInt[7] = {2, 10, NUM_LEDS-5, 0, 0, 0, 255};
  unsigned long zipParamUnsignedLong[1] = {20};
  int zipPosCounter = 0;
  unsigned long zipLastActivate = 0;

  int flashToBeatParamInt[4] = {255,255,255,50};
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

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef (&animParamRefArray)[NB_ARRAYS]);

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