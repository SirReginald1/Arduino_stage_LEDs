#include <stdint.h>
#include <sys/types.h>
/**
  * ESP32 Animations
*/

#include "Arduino.h"
#include <FastLED.h>
#include "Globals.h"
#include "Com_interface.h"
#include "Beat_detector.h"
#include "Params.h"


extern float* timings; 
extern int timingsLength;

/** The value indicating if the mic and FFT loop is runing on core0. */
extern bool appModeMicFFTOnCore1;

/** The current time */
extern unsigned long millisecs;

/** The notification queue used by core0 to signal core 1 */
extern QueueHandle_t core1NotifQueue;

/** Value indicating what audio feature hase been detected. If none detected will be set to NO_AUDIO_FEATURE_DETECTED. */
extern u_int16_t FFTAudioFeatureDetected;
/** Indicates if a kick is detected */
extern bool detectedKick;
/** Indicates if a snaire is detected */
extern bool detectedSnaire;
/** Indicates if a high hat is detected */
extern bool detectedHighHat;

int avtivatedArrays = 0;

// ###########################################################################################################################
// #################################################### UTILITY FUNCTIONS ####################################################
// ###########################################################################################################################

/**
* Calculates a linear ramp value for a givent position on the ramp.
*
* @param pos The possition on the ramp (array).
* @param floor The minimum value that can be taken by the ramp.
* @param ceiling The maximum value that can be taken by the ramp.
* @param nbLeds The number of positions in the ramp (lenght  of array).
*/
float ramp(int& pos, int& floor, int& ceiling, const int& nbLeds){
  return min(max((float)((pos) / nbLeds), (float)floor), (float)ceiling);
}

/**
* Sets the linear ramp values for a givent ramp value array.
*
* @param rampArray The array to have its values reset.
* @param floor The minimum value that can be taken by the ramp.
* @param ceiling The maximum value that can be taken by the ramp.
* @param nbLeds The number of positions in the ramp (lenght  of array).
*/
void setRampValues(float* rampArray, int& floor, int& ceiling, const int& nbLeds){
  for(int i=0; i<nbLeds; i++){
    rampArray[i] = ramp(i, floor, ceiling, nbLeds);
  }
}

/**
 * Places particles equaly spaced depending on the number of particles used and places them in the position table.
 * Function to be called when switching annimations and befor animation playing more generaly.
 * 
 * @param particlePosArray The array referencing the position of each used particle.
 * @param nbParticles The numper of particles to place in the array.
 * @param nbLeds The number of leds present in the array.
 */
void setParticleConverge1DPosArray(int* particlePosArray, int& nbParticles, int nbLeds){
  int spacing = nbLeds / (nbParticles + 1); // Because there is one more space than there are particles.
  int position = spacing;
  for(int i=0; i<nbParticles; i++){
    particlePosArray[i] = position;
    position += spacing;
  }
}

  //int particleConverge1DParamInt[6] = {3,0,0,255,0,0}; // {nbParticles, flore, ceiling, red, green, blue}
  //unsigned long particleConverge1DParamUnsignedLong[1] = {20}; // Speed
  /** The array containing the list of all the particle positions. values of -1 mean the that particle is not in use therefor end loop*/
  //int* particleConverge1DParticlePos;
  //int particleConverge1DMaxNbParticles;
  //float* particleConverge1DRampValues;
  //int particleConverge1DStep = 0;
  //unsigned long particleConverge1DLastActivate = 0;

  // Constructor
  //animParamRef(int ledCount = NUM_LEDS, u_int16_t arrayIdentifier = avtivatedArrays): nbLeds(ledCount), arrayId(arrayIdentifier) {
  //  // Set ceiling
  //  particleConverge1DParamInt[2] = nbLeds;
  //  // Allocate memory for fireHeat array
  //  fireHeat = new byte[nbLeds]{0};  
  //  // Allocate memory for particleConverge1DParticlePos array
  //  particleConverge1DParticlePos = new int[nbLeds/2]; // This allows for a maximum of 1 particle every other pixel
  //  setParticleConverge1DPosArray(particleConverge1DParticlePos, particleConverge1DParamInt[0], nbLeds);
  //  //particleConverge1DMaxNbParticles = nbLeds/2;
  //  // Calculate ramp values at construcion
  //  particleConverge1DRampValues = new float[nbLeds];
  //  setRampValues(particleConverge1DRampValues, particleConverge1DParamInt[0], particleConverge1DParamInt[1],nbLeds);
  //  avtivatedArrays ++; // !!!!!!!!!!!!!!!!!!!! Not sure this will work !!!!!!!!!!!!!!!!!
  //}

// Destructor to prevent memory leaks
  //~animParamRef() {
  //  // We only explicitly reference these as they are the only 2 variables that are dynamically allocated.
  //  delete[] fireHeat;
  //  delete[] particleConverge1DParticlePos;
  //  delete[] particleConverge1DRampValues;
  //}


/*
struct multiAnimArrayVars{
  // int maxNbArrayForAnim;
  // int minNbArrayForAnim;
  // u_int16_t* arraysInAnim; // Contains list arrayIds that
};
*/

/**
 * Creates a new animParamRef struct with the given length.
 * 
 * @param nbLeds The number of leds contained in the array.
 */
//animParamRef newAnimParamRef(int nbLeds){
//  animParamRef out;
//  out.nbLeds = nbLeds;
//  out.fireHeat = new byte[nbLeds];
//  return out;
//}

/*
void syncAnimation(animParamRef[NB_ARRAYS]){

}
*/

class Animations{
  public:

// #########################################################################
// ############################## Class variables ##########################
// #########################################################################
    /** Indicates if the flashToBeat animation should start. */
    static bool flashToBeatGo;

    /** Indicates if the flashToBeat animation has started. */
    static bool flashToBeatStarted;

// #########################################################################
// ##################### Animation Parameter References ####################
// #########################################################################
    //template <typesname T> T getParameters(int animationCode, animParamRef animationStruct);

    /**
      * Returns the array of integer parameters for specified animation.
      *
      * @param animationCode The code for the animation the integer parameters of which are to be retreved.
      * @param animationStruct The struct that references the integer parameter values for a particular LED array.
      * @return The array of integer parameters for the specified animation in order of specification in function signiature.
    */
    static int* getParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    /**
      * Returns the array of float parameters for specified animation.
      *
      * @param animationCode The code for the animation the float parameters of which are to be retreved.
      * @param animationStruct The struct that references the float parameter values for a particular LED array.
      * @return The array of float parameters for the specified animation in order of specification in function signiature.
    */
    static float* getParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    /**
      * Returns the array of unsigned long parameters for specified animation.
      *
      * @param animationCode The code for the animation the unsigned long parameters of which are to be retreved.
      * @param animationStruct The struct that references the unsigned long parameter values for a particular LED array.
      * @return The array of unsigned long parameters for the specified animation in order of specification in function signiature.
    */
    static unsigned long* getParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode);

    /**
      * Sets the value of a parameter in the parameter struct array to the given value.
      *
      * @param animationStruct The parameter struc array to be modified.
      * @param arrayIdx The index indicating which arrays struct is to be modified.
      * @param animationCode The code representing the animation to ba changed.
      * @param paramIdx The index indicating which integer parameter is to be changed. The parameter are in order of specification in function signiature.
      * @param paramValue The value the parameter is to be switched to.
    */
    static void setParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, int paramValue);

    /**
      * Sets the value of a parameter in the parameter struct array to the given value.
      *
      * @param animationStruct The parameter struc array to be modified.
      * @param arrayIdx The index indicating which arrays struct is to be modified.
      * @param animationCode The code representing the animation to ba changed.
      * @param paramIdx The index indicating which integer parameter is to be changed. The parameter are in order of specification in function signiature.
      * @param paramValue The value the parameter is to be switched to.
    */
    static void setParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, float paramValue);

    /**
      * Sets the value of a parameter in the parameter struct array to the given value.
      *
      * @param animationStruct The parameter struc array to be modified.
      * @param arrayIdx The index indicating which arrays struct is to be modified.
      * @param animationCode The code representing the animation to ba changed.
      * @param paramIdx The index indicating which integer parameter is to be changed. The parameter are in order of specification in function signiature.
      * @param paramValue The value the parameter is to be switched to.
    */
    static void setParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, unsigned long paramValue);

    /**
     * Sets the toSet array with all the animation variables present in the reference struct.
     * 
     * @param animationStruct The parameter struc array to be modified. 
     * @param reference The index of the reference 
     */
    static void synchAnimVarAll(animParamRef animationStruct[NB_ARRAYS], int reference, int toSet);

// #########################################################################
// ############################ Run animations #############################
// #########################################################################

    /** The array containing all the animation functions. */
    static void (*animations[NB_ANIMATIONS])(CRGB*, StackAnimParamRef&);

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], StackAnimParamRef (&animParamRefArray)[NB_ARRAYS]);

// #########################################################################
// ####################### Rainbow Cycle animation #########################
// #########################################################################

    static byte *Wheel(byte WheelPosition) {
      static byte c[3];

      if(WheelPosition < 85) {
       c[0] = WheelPosition * 3;
       c[1] = 255 - WheelPosition * 3;
       c[2] = 0;
      }
      else if(WheelPosition < 170) {
       WheelPosition -= 85;
       c[0] = 255 - WheelPosition * 3;
       c[1] = 0;
       c[2] = WheelPosition * 3;
      }
      else {
       WheelPosition -= 170;
       c[0] = 0;
       c[1] = WheelPosition * 3;
       c[2] = 255 - WheelPosition * 3;
      }

      return c;
    }

    static void rainbowCycle(CRGB* leds, StackAnimParamRef& parameters);

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

    static void fadeInAndOut(CRGB* leds, StackAnimParamRef& parameters);

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

    static void sparkle(CRGB* leds, StackAnimParamRef& parameters);

// #########################################################################
// ############################ Fire animation #############################
// #########################################################################

    // FlameHeight - Use larger value for shorter flames, default=50.
    // Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
    // DelayDuration - Use larger value for slower flame speed, default=10.
    static void Fire(CRGB* leds, StackAnimParamRef& parameters);

    static void setPixelHeatColor(CRGB* leds, int Pixel, byte temperature, float intensity) {
      // Rescale heat from 0-255 to 0-191
      byte t192 = round((temperature / 255.0) * 191);

      // Calculate ramp up from
      byte heatramp = t192 & 0x3F; // 0...63
      heatramp <<= 2; // scale up to 0...252

      // Figure out which third of the spectrum we're in:
      if(t192 > 0x80) {                    // hottest
        leds[Pixel].setRGB(255*intensity, heatramp*intensity, 255*intensity);
      }
      else if(t192 > 0x40) {               // middle
        leds[Pixel].setRGB(heatramp*intensity, 255*intensity, 0);
      }
      else {                               // coolest
        leds[Pixel].setRGB(0, heatramp*intensity, 0);
      }
    }

// #########################################################################
// ######################## Shooting Star animation ########################
// #########################################################################

    /* 
     *  ========== Shooting Star Animation ==========
     *  red, green, blue - Choose a color with RGB values (0 to 255).
     *  tail_length - A larger value results in shorter tail.
     *  delay_duration - A larger value results in slower movement speed.
     *  interval - Time interval between new shooting stars (in milliseconds).
    */
    static void shootingStar(CRGB* leds, StackAnimParamRef& parameters);

// #########################################################################
// ######################## Twinkle Pixels animation #######################
// #########################################################################

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
    static void twinklePixels(CRGB* leds, StackAnimParamRef& parameters);

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

    static void strobe(CRGB* leds, StackAnimParamRef& parameters);

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

    static void zip(CRGB* leds, StackAnimParamRef& parameters);

// #########################################################################
// ######################## Clear Sequential animation #####################
// #########################################################################

    /*
      * Clears the whole strip sequentialy in the specified direction.
      * 
      * @param leds The array of leds on which to opperate.
      * @param direction If 0 will run from plug to end. Else will run from top to plug.
      * @param speed Length of the delay in millisecs between each led turning off.
    */
    static void clear_sequential(CRGB* leds, int direction, int speed){
      if(direction == 0){
        for(int i=0;i<NUM_LEDS;i++){
          leds[i] =  CRGB::Black;
          FastLED.show();
          delay(speed);
        }
      }
      else{
        for(int i=NUM_LEDS;i>-1;i--){
          leds[i] =  CRGB::Black;
          FastLED.show();
          delay(speed);
        }
      }

    }


// #############################################################################################################
// ######################################## particle converge 1D ###############################################
// #############################################################################################################

    //static void setParticleConverge1DPosArray(int* particlePosArray, int& nbParticles, int& nbLeds);

    static void particleConverge1D(CRGB* leds, StackAnimParamRef& parameters);

// #############################################################################################################
// ############################################ BEAT ANIMATIONS ################################################
// #############################################################################################################

    static void flashToBeat(CRGB* leds, StackAnimParamRef& parameters);

// #############################################################################################################
// ############################################ PREPREP ANIMATIONS #############################################
// #############################################################################################################

    static void flashToBeatArray(CRGB leds[NB_ARRAYS][NUM_LEDS], float* timings, int* timingsLength, unsigned long current_time, CRGB color);

    static void stopFlashToBeatArray();

  /*  
  void electromagneticSpectrum(int transitionSpeed) {
      switch(colorTime) {
        case 1:
          greenColor += transitionSpeed;
          blueColor -= transitionSpeed;
          if (greenColor >= 255 or blueColor <= 0) {
            greenColor = 255;
            blueColor = 0;
            colorTime = 2;
          }
          break;
        case 2:
          redColor += transitionSpeed;
          greenColor -= transitionSpeed;
          if (redColor >= 255 or greenColor <= 0) {
            redColor = 255;
            greenColor = 0;
            colorTime = 3;
          }
          break;
        case 3:
          redColor -= transitionSpeed;
          blueColor += transitionSpeed;
          if (redColor <= 0 or blueColor >= 255) {
            redColor = 0;
            blueColor = 255;
            colorTime = 1;
          }
          break;
      }
  }*  /

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
};

// ###########################################################################################################################
// ############################################### STATIC VARIABLE DEFINITIONS ###############################################
// ###########################################################################################################################

void (*Animations::animations[NB_ANIMATIONS])(CRGB*, StackAnimParamRef&) = {&Animations::rainbowCycle, 
                                                                            &Animations::fadeInAndOut, 
                                                                            &Animations::sparkle,
                                                                            &Animations::Fire,
                                                                            &Animations::shootingStar,
                                                                            &Animations::twinklePixels,
                                                                            &Animations::strobe,
                                                                            &Animations::zip,
                                                                            &Animations::flashToBeat//,
                                                                       //&Animations::particleConverge1D
                                                                      };


//Animations::animations[ANIM_CODE_RAINBOWCYCLE] = &Animations::rainbowCycle;
//Animations::animations[ANIM_CODE_SPARKLE] = &Animations::sparkle;
//Animations::animations[ANIM_CODE_FIRE] = &Animations::Fire;


bool Animations::flashToBeatGo = false;

bool Animations::flashToBeatStarted = false;

/**
* Function that must be called by any action that interupts the animation.
*/
void Animations::stopFlashToBeatArray(){
  Animations::flashToBeatGo = false;
  Animations::flashToBeatStarted = false;
}

// ###########################################################################################################################
// ############################################### STATIC DEFINITIONS ########################################################
// ###########################################################################################################################

/**
 * Runs the animations for the given arrays using the parameters contained in the 
 * 
 * @param ledArrays Array containing all the individual led arrays.
 * @param animParamRefArray The array containing all the parameters structs.
 */
void Animations::runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], StackAnimParamRef (&animParamRefArray)[NB_ARRAYS]){
  for(int i=0;i<NB_ARRAYS;i++){
    // Add animParamRef selection for animation synching here
    (*animations[animParamRefArray[i].animation])(ledArrays[i], animParamRefArray[i]);
  }
  FastLED.show();
}

// #########################################################################
// ##################### Animation Parameter References ####################
// #########################################################################
/*
    int* Animations::getParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case ANIM_CODE_RAINBOWCYCLE:
          return animationStruct[arrayIdx].rainbowCycleParamInt;
          break;
        case ANIM_CODE_FADE:
          return animationStruct[arrayIdx].fadeInAndOutParamInt;
          break;
        case ANIM_CODE_SPARKLE:
          return animationStruct[arrayIdx].sparkleParamInt;
          break;
        case ANIM_CODE_FIRE:
          return animationStruct[arrayIdx].fireParamInt;
          break;
        case ANIM_CODE_SHOOTING_STAR:
          return animationStruct[arrayIdx].shootingStarParamInt;
          break;
        case ANIM_CODE_TWINKLE_PIXELS:
          return animationStruct[arrayIdx].twinklePixelsParamInt;
          break;
        case ANIM_CODE_STROBE:
          return animationStruct[arrayIdx].strobeParamInt;
          break;
        case ANIM_CODE_ZIP:
          return animationStruct[arrayIdx].zipParamInt;
          break;
        case ANIM_CODE_FLASH_TO_BEAT:
          return animationStruct[arrayIdx].flashToBeatParamInt;
          break;
        //case ANIM_CODE_PARTICLE_CONVERGE_1D:
        //  return animationStruct[arrayIdx].particleConverge1DParamInt;
        //  break;
      }
    }

    float* Animations::getParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case ANIM_CODE_FIRE:
          return animationStruct[arrayIdx].fireParamFloat;
          break;
      }
    }

    unsigned long* Animations::getParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case ANIM_CODE_ZIP:
          return animationStruct[arrayIdx].zipParamUnsignedLong;
          break;
        //case ANIM_CODE_PARTICLE_CONVERGE_1D:
        //  return animationStruct[arrayIdx].particleConverge1DParamUnsignedLong;
        //  break;
      }
    }

    void Animations::setParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, int paramValue){
      switch (animationCode){
        case ANIM_CODE_RAINBOWCYCLE:
          animationStruct[arrayIdx].rainbowCycleParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_FADE:
          animationStruct[arrayIdx].fadeInAndOutParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_SPARKLE:
          animationStruct[arrayIdx].sparkleParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_FIRE:
          animationStruct[arrayIdx].fireParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_SHOOTING_STAR:
          animationStruct[arrayIdx].shootingStarParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_TWINKLE_PIXELS:
          animationStruct[arrayIdx].twinklePixelsParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_STROBE:
          animationStruct[arrayIdx].strobeParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_ZIP:
          animationStruct[arrayIdx].zipParamInt[paramIdx] = paramValue;
          break;
        case ANIM_CODE_FLASH_TO_BEAT:
          animationStruct[arrayIdx].flashToBeatParamInt[paramIdx] = paramValue;
          break;
        //case ANIM_CODE_PARTICLE_CONVERGE_1D:
        //  static int par1Changed = -1;
        //  switch (paramIdx){
        //    case 0:
        //      // Initialize the particle position array when switching parameters if parameter has changed.
        //      if(paramValue != animationStruct[arrayIdx].particleConverge1DParamInt[0]){
        //        setParticleConverge1DPosArray(animationStruct[arrayIdx].particleConverge1DParticlePos, paramValue, animationStruct[arrayIdx].nbLeds);
        //      }
        //      break;
        //    case 1:
        //      // If parameter at pos 1 has chaged save value.
        //      if(paramValue != animationStruct[arrayIdx].particleConverge1DParamInt[1]){
        //        par1Changed = paramValue;
        //      }
        //      break;
        //    case 2:
        //      // Recalculate ramp values if any of the parameters have changed.
        //      if((paramValue != animationStruct[arrayIdx].particleConverge1DParamInt[2]) | par1Changed != -1){
        //        setRampValues(animationStruct[arrayIdx].particleConverge1DRampValues, par1Changed, paramValue, animationStruct[arrayIdx].nbLeds);
        //        par1Changed = -1;
        //      }
        //      break;
        //    default:
        //      break;
        //  }
        //  animationStruct[arrayIdx].particleConverge1DParamInt[paramIdx] = paramValue;
        //  break;
      }
    }

    void Animations::setParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, float paramValue){
      switch (animationCode){
        case ANIM_CODE_FIRE:
          animationStruct[arrayIdx].fireParamFloat[paramIdx] = paramValue;
          break;
      }
    }

    void Animations::setParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, unsigned long paramValue){
      switch (animationCode){
        case ANIM_CODE_ZIP:
          animationStruct[arrayIdx].zipParamUnsignedLong[paramIdx] = paramValue;
          break;
        //case ANIM_CODE_PARTICLE_CONVERGE_1D:
        //  animationStruct[arrayIdx].particleConverge1DParamUnsignedLong[0] = paramValue;
        //  break;
      }
    }
*/
// #########################################################################
// ####################### Rainbow Cycle animation #########################
// #########################################################################

/**
  * Rainbow Circle animation that is ment to run with an array of LED arrays. All arrays ars synced.
*/
void Animations::rainbowCycle(CRGB* leds, StackAnimParamRef& parameters){
  byte *c;
  uint16_t i;
  if(millisecs - parameters.timeLastActivate > (unsigned long)parameters.intAdjustParams[0]){
    if(parameters.intParams[0] > 256){
      parameters.intParams[0] = 0;
    }
    for(i=0; i < parameters.nbLeds; i++) { // This makes sure all LEDs on strip are updated
      c = Wheel(((i * 256 / parameters.nbLeds) + /*j*/ parameters.intParams[0]) & 255);
      leds[parameters.nbLeds - 1 - i].setRGB(*c, *(c+1), *(c+2));
    }
    parameters.intParams[0]++;
    parameters.timeLastActivate = millisecs;
  }
}

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

void Animations::fadeInAndOut(CRGB* leds, StackAnimParamRef& parameters){//, int randomColors){
  float r, g, b;
  // FADE IN
  //for(int i = 0; i <= 255; i++) {
  if(millisecs - parameters.timeLastActivate > parameters.uLongAdjustParams[0]){
    r = (parameters.intParams[0]/256.0)*parameters.intAdjustParams[0];
    g = (parameters.intParams[0]/256.0)*parameters.intAdjustParams[1];
    b = (parameters.intParams[0]/256.0)*parameters.intAdjustParams[2];
    fill_solid(leds, parameters.nbLeds, CRGB(r, g, b));
    if(parameters.boolParams[0]){
      parameters.intParams[0]++;
    }
    else{
      parameters.intParams[0]--;
    }
    if(parameters.intParams[0] == 25){
      parameters.boolParams[0] = false;
    }
    else if(parameters.intParams[0] == 0){
      parameters.boolParams[0] = true;
    }
    parameters.timeLastActivate = millisecs;
  }
}

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

void Animations::sparkle(CRGB* leds, StackAnimParamRef& parameters) {
  EVERY_N_MILLISECONDS(parameters.intAdjustParams[3]){
    leds[parameters.intParams[0]].setRGB(0, 0, 0);
    parameters.intParams[0] = random(parameters.nbLeds);
    leds[parameters.intParams[0]].setRGB(parameters.intAdjustParams[1], parameters.intAdjustParams[0], parameters.intAdjustParams[2]);
  }
} 

// #########################################################################
// ############################ Fire animation #############################
// #########################################################################

/**
  * {int flame hight, int sparks, int delay, float intensity}
  * FlameHeight - Use larger value for shorter flames, default=50.
  * Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
  * DelayDuration - Use larger value for slower flame speed, default=10.
*/
void Animations::Fire(CRGB* leds, StackAnimParamRef& parameters) {
  int cooldown;
  // Cool down each cell a little
  if(millisecs - parameters.timeLastActivate > parameters.uLongAdjustParams[0]){
    for(int i = 0; i < parameters.nbLeds; i++) {
      cooldown = random(0, ((parameters.intAdjustParams[0] * 10) / parameters.nbLeds) + 2);
      if(cooldown > parameters.byteParams[i]) {
        parameters.byteParams[i] = 0;
      }
      else {
        parameters.byteParams[i] = parameters.byteParams[i] - cooldown;
      }
    }
    // Heat from each cell drifts up and diffuses slightly
    for(int k = (parameters.nbLeds - 1); k >= 2; k--) {
      parameters.byteParams[k] = (parameters.byteParams[k - 1] + parameters.byteParams[k - 2] + parameters.byteParams[k - 2]) / 3;
    }
    // Randomly ignite new Sparks near bottom of the flame
    if(random(255) < parameters.intAdjustParams[1]) {
      int y = random(7);
      parameters.byteParams[y] = parameters.byteParams[y] + random(160, 255);
    }
    // Convert heat to LED colors
    for(int j = 0; j < parameters.nbLeds; j++) {
      setPixelHeatColor(leds, j, parameters.byteParams[j], parameters.floatAdjustParams[0]);
    }
    parameters.timeLastActivate = millisecs;
  }
}

// #########################################################################
// ######################## Shooting Star animation ########################
// #########################################################################

/** 
 *  ========== Shooting Star Animation ==========
 *  red, green, blue - Choose a color with RGB values (0 to 255).
 *  tail_length - Value which represents number of pixels used in the tail following the shooting star. A larger value results in shorter tail.
 *  delay_duration - value to set animation speed. Higher value results in slower animation speed. A larger value results in slower movement speed.
 *  interval - Time interval between new shooting stars (in milliseconds).
 * direction - value which changes the way that the pixels travel (uses -1 for reverse, any other number for forward)
*/
void Animations::shootingStar(CRGB* leds, StackAnimParamRef& parameters){
  if(millisecs - parameters.timeLastActivate >= parameters.uLongAdjustParams[0]){ // Animation speed
    if (millisecs - parameters.uLongParams[0] >= parameters.uLongAdjustParams[1]) { // Shooting star interval
      parameters.uLongParams[0] = millisecs;         // Save the last time the LEDs were updated
      //shootingStarCount = 0;
      parameters.intParams[0] = 0;                              // Reset the count to 0 after each interval
    }
    if (parameters.intAdjustParams[4] == -1) {        // Reverse direction option for LEDs
      if (parameters.intParams[0] < parameters.nbLeds) {
        leds[parameters.nbLeds - (parameters.intParams[0] % (parameters.nbLeds+1))].setRGB(parameters.intAdjustParams[0], parameters.intAdjustParams[1], parameters.intAdjustParams[2]);    // Set LEDs with the color value
        parameters.intParams[0]++;
      }
    }
    else {
      if (parameters.intParams[0] < parameters.nbLeds) {     // Forward direction option for LEDs
        leds[parameters.intParams[0] % (parameters.nbLeds+1)].setRGB(parameters.intAdjustParams[0], parameters.intAdjustParams[1], parameters.intAdjustParams[2]);    // Set LEDs with the color value
        parameters.intParams[0]++;
      }
    }
    fadeToBlackBy(leds, parameters.nbLeds, parameters.intAdjustParams[3]);                 // Fade the tail LEDs to black
  }
  parameters.timeLastActivate = millisecs;
}

// #########################################################################
// ######################## Twinkle Pixels animation #######################
// #########################################################################

/**
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
void Animations::twinklePixels(CRGB* leds, StackAnimParamRef& parameters) {
  if(millisecs - parameters.timeLastActivate > parameters.uLongAdjustParams[0]){
    for (int i = 0; i < parameters.nbLeds; i++) {
      // Draw twinkling pixels
      if (random(parameters.intAdjustParams[2]) < 2) {     // Chance for pixel to twinkle
        uint8_t intensity = random(100, 255);     // Configure random intensity
        //CHSV(Color, ColorSaturation, intensity);     // Configure color with max saturation and variable value (intensity)
        leds[i].setHSV(parameters.intAdjustParams[0], parameters.intAdjustParams[1], intensity);    // Set the pixel color
      }
      // Fade LEDs
      if (leds[i].r > 0 || leds[i].g > 0 || leds[i].b > 0) {    // Check if pixel is lit
        leds[i].fadeToBlackBy(parameters.intAdjustParams[3]);    // Fade pixel to black
      }
    }
    parameters.timeLastActivate = millisecs;
  }
}

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

void Animations::strobe(CRGB* leds, StackAnimParamRef& parameters){
  if(millisecs - parameters.timeLastActivate > parameters.uLongAdjustParams[0] + parameters.uLongAdjustParams[1]){
    parameters.timeLastActivate = millisecs;
    fill_solid(leds, parameters.nbLeds, CRGB(parameters.intAdjustParams[0], parameters.intAdjustParams[2], parameters.intAdjustParams[3]));
  }
  else if (millisecs - parameters.uLongParams[0] > parameters.uLongAdjustParams[0]) {
    parameters.uLongParams[0] = millisecs;
    FastLED.clear();
  }
}

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

void Animations::zip(CRGB* leds, StackAnimParamRef& parameters){
  //static unsigned long zip_animation_prev_mills = 0;
  //static int zip_animation_pos_counter = 0;
  int startPos, endPos;
  if(millisecs - parameters.timeLastActivate > parameters.uLongAdjustParams[0]){
    // This is the start of the strip
    startPos = parameters.intParams[0] + parameters.intAdjustParams[1];
    if(startPos <= parameters.nbLeds && startPos < parameters.intAdjustParams[2]){
      leds[startPos].setRGB(parameters.intAdjustParams[4], parameters.intAdjustParams[5], parameters.intAdjustParams[6]);
    }
    // This is the end of the strip
    endPos = parameters.intParams[0] + parameters.intAdjustParams[1] - parameters.intAdjustParams[0];
    if(endPos >= 0 && endPos <= parameters.nbLeds){
      leds[endPos].setRGB(0,0,0);
    }
    //FastLED.show();
    // Only 1 element so the step is the position of the element
    parameters.intParams[0] = ++parameters.intParams[0] % (parameters.intAdjustParams[2] - parameters.intAdjustParams[1] + parameters.intAdjustParams[0]);
    parameters.timeLastActivate = millisecs;
  }
} 

// ############################################################################
// ###################### particleConverge1D animation ########################
// ############################################################################

//void Animations::particleConverge1D(CRGB* leds, animParamRef& parameters){
//  // Only activate if appropriate amount of time has passed (speed)
//  if(millisecs - parameters.particleConverge1DLastActivate > parameters.particleConverge1DParamUnsignedLong[0]){
//    static int idx_particle_pos = 0;
//    // Loop through all active particles (-1 means that particle is not active)
//    while(idx_particle_pos < parameters.particleConverge1DParamInt[0]){
//      static int currentParticlePosition = parameters.particleConverge1DParticlePos[idx_particle_pos];
//      // Check not edge cases (only look right so only check right edge case)
//      //if(currentParticlePosition < parameters.nbLeds){
//        //static int current_pos = parameters.particleConverge1DParticlePos[idx_particle_pos];
//        static int next_pos = currentParticlePosition + 1;
//        static float next_ramp_value = parameters.particleConverge1DRampValues[next_pos];
//        leds[next_pos].r = parameters.particleConverge1DParamInt[2] * next_ramp_value;
//        leds[next_pos].g = parameters.particleConverge1DParamInt[3] * next_ramp_value;
//        leds[next_pos].b = parameters.particleConverge1DParamInt[4] * next_ramp_value;
//        // Cycle through variables
//        parameters.particleConverge1DParticlePos[idx_particle_pos] = (parameters.particleConverge1DParticlePos[idx_particle_pos] + 1) % parameters.nbLeds;
//        //idx_particle_pos = (idx_particle_pos + 1) % parameters.particleConverge1DParamInt[0];
//      //}
//    }
//    fadeToBlackBy(leds, parameters.nbLeds, parameters.particleConverge1DParamInt[3]);
//    parameters.animationPosition[ANIM_CODE_PARTICLE_CONVERGE_1D] = (parameters.animationPosition[ANIM_CODE_PARTICLE_CONVERGE_1D] + 1) % parameters.nbLeds; // !!!!! Check on this to make sure that it enables proper syncinig !!!!
//    parameters.particleConverge1DLastActivate = millisecs;
//  }
//}

// ############################################################################
// ########################## flashToBeat animation ###########################
// ############################################################################

void Animations::flashToBeat(CRGB* leds, StackAnimParamRef& parameters){
  if(detectedKick){
    //Serial.print("Beat type: ");Serial.println(beatType);
    fill_solid(leds, parameters.nbLeds, CRGB(parameters.intAdjustParams[1], parameters.intAdjustParams[0], parameters.intAdjustParams[2]));
  }
  if(millisecs - parameters.timeLastActivate > parameters.intAdjustParams[3]){
    fill_solid(leds, parameters.nbLeds, CRGB(0, 0, 0));
    parameters.timeLastActivate = millisecs;
  }
}


// #############################################################################################################
// ############################################ PREPREP ANIMATIONS #############################################
// #############################################################################################################

/**
  * Flashes all the arrays in time with the beat given by the timings array.
  *
  * @param leds The array of led arrays.
  * @param timings The array containing the timings in milliseconds for all the beats in a given music.
  * @param timingsLength The lenght of the beat array.
  * @param current_time The current time in milliseconds.
  * @param color The CRDB object containing the color values for the arrays.
*/
void Animations::flashToBeatArray(CRGB leds[NB_ARRAYS][NUM_LEDS], float* timings, int* timingsLength, unsigned long current_time, CRGB color){
  if(!timings){
    Serial.println("The array containing the beats is null. Can't run animation.");
    return;
  }
  static int i;
  static unsigned long start;
  if(Animations::flashToBeatGo){
      if(!Animations::flashToBeatStarted){
        start = millis();
        i = 0;
        //Serial.print("Started set: ");Serial.println(start);
        Animations::flashToBeatStarted = true;
      }
        //Serial.print("Timing ");Serial.print(i);Serial.print(": ");Serial.print("millisecs - start: ");Serial.print(millisecs - start);Serial.print(" | timings[i]: ");Serial.println(timings[i]);
        //millisecs = millis();
        if((current_time - start) > timings[i]){
          //Serial.print("Timing ");Serial.print(i);Serial.print(": ");Serial.print("millisecs: ");Serial.print(millisecs);Serial.print(" | millisecs - start: ");Serial.print(millisecs - start);Serial.print(" | timings[i]: ");Serial.println(timings[i]);
          for(int l=0;l<NB_ARRAYS;l++){
            fill_solid(leds[l], NUM_LEDS, color);
          }
          //Serial.println("Flash");
          FastLED.show();
          for(int l=0;l<NB_ARRAYS;l++){
            fill_solid(leds[l], NUM_LEDS, CRGB(0,0,0));
          }
          FastLED.show();
          i++;
        }
      if(i >= *timingsLength){
        Animations::flashToBeatGo = false;
        Animations::flashToBeatStarted = false;
      }
  }
}
