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

/**Struct that contains the number of the animation being run by the array it's attributed to
 *  as well as all the references to the aniamtion parameters.
 * */
struct animParamRef{

  /** This variable is used to idetify the array when using multi-array animations  */
  u_int16_t arrayId;

  int nbLeds = NUM_LEDS; // The number of leds in the assigned array

  int animation = 0; // The animation the array is currentlly set to

  int rainbowCycleParamInt[1] = {0}; // {int delay}
  int rainbowCycleVarInt[1] = {0}; // {point in animation}
  unsigned long rainbowCycleLastActivate = 0;

  int fadeInAndOutParamInt[4] = {255, 255, 255, 100}; // {int red, int green, int blue, int speed}
  int fadeInAndOutFadeAmount = 0;
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
  int shootingStarPosCounter = 0;

  int twinklePixelsParamInt[5] = {200, 50, 50, 100, 50}; // {int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration}
  unsigned long twinklePixelsLastActivate = 0;

  int strobeParamInt[5] = {20, 55, 255, 255, 255}; // {int time_on, int time_off, int R, int G, int B}
  unsigned long strobeLastOn = 0;
  unsigned long strobeLastOff = 0;

  int zipParamInt[7] = {2, 10, NUM_LEDS-5, 0, 0, 0, 255}; // {int size, int start, int end, int delay, int R, int G, int B}
  unsigned long zipParamUnsignedLong[1] = {20}; // {unsigned long speed, unsigned long current_time}
  int zipPosCounter = 0;
  unsigned long zipLastActivate = 0;

  int flashToBeatParamInt[4] = {255,255,255,50}; // {red, green, blue, time_left_on}
  bool flashToBeatIsOn = false;
  unsigned long flashToBeatLastActivate = 0;
};

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
animParamRef newAnimParamRef(int nbLeds){
  animParamRef out;
  out.nbLeds = nbLeds;
  out.fireHeat = new byte[nbLeds];
  return out;
}

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

// #########################################################################
// ############################ Run animations #############################
// #########################################################################

    /** The array containing all the animation functions. */
    static void (*animations[NB_ANIMATIONS])(CRGB*, animParamRef&);

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef (&animParamRefArray)[NB_ARRAYS]);

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

    static void rainbowCycle(CRGB* leds, animParamRef& parameters);

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

    static void fadeInAndOut(CRGB* leds, animParamRef& parameters);

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

    static void sparkle(CRGB* leds, animParamRef& parameters);

// #########################################################################
// ############################ Fire animation #############################
// #########################################################################

    // FlameHeight - Use larger value for shorter flames, default=50.
    // Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
    // DelayDuration - Use larger value for slower flame speed, default=10.
    static void Fire(CRGB* leds, animParamRef& parameters);

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
    static void shootingStar(CRGB* leds, animParamRef& parameters);

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
    static void twinklePixels(CRGB* leds, animParamRef& parameters);

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

    static void strobe(CRGB* leds, animParamRef& parameters);

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

    static void zip(CRGB* leds, animParamRef& parameters);

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
// ############################################ BEAT ANIMATIONS ################################################
// #############################################################################################################

    static void flashToBeat(CRGB* leds, animParamRef& parameters);

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

void (*Animations::animations[NB_ANIMATIONS])(CRGB*, animParamRef&) = {&Animations::rainbowCycle, 
                                                                       &Animations::fadeInAndOut, 
                                                                       &Animations::sparkle,
                                                                       &Animations::Fire,
                                                                       &Animations::shootingStar,
                                                                       &Animations::twinklePixels,
                                                                       &Animations::strobe,
                                                                       &Animations::zip,
                                                                       &Animations::flashToBeat};


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
void Animations::runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef (&animParamRefArray)[NB_ARRAYS]){
  for(int i=0;i<NB_ARRAYS;i++){
    // Add animParamRef selection for animation synching here
    (*animations[animParamRefArray[i].animation])(ledArrays[i], animParamRefArray[i]);
  }
  FastLED.show();
}

// #########################################################################
// ##################### Animation Parameter References ####################
// #########################################################################

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
        case ANIM_CODE_SHOUTING_STAR:
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
      }
      return 0;
    }

    float* Animations::getParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case ANIM_CODE_FIRE:
          return animationStruct[arrayIdx].fireParamFloat;
          break;
      }
      return 0;
    }

    unsigned long* Animations::getParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case ANIM_CODE_ZIP:
          return animationStruct[arrayIdx].zipParamUnsignedLong;
          break;
      }
      return 0;
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
        case ANIM_CODE_SHOUTING_STAR:
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
      }
    }

// #########################################################################
// ####################### Rainbow Cycle animation #########################
// #########################################################################

/**
  * Rainbow Circle animation that is ment to run with an array of LED arrays. All arrays ars synced.
*/
void Animations::rainbowCycle(CRGB* leds, animParamRef& parameters){
  byte *c;
  uint16_t i;
  if(millisecs - parameters.rainbowCycleLastActivate > (unsigned long)parameters.rainbowCycleParamInt[0]){
    if(parameters.rainbowCycleVarInt[0] >256){
      parameters.rainbowCycleVarInt[0] = 0;
    }
    for(i=0; i < parameters.nbLeds; i++) { // This makes sure all LEDs on strip are updated
      c = Wheel(((i * 256 / parameters.nbLeds) + /*j*/ parameters.rainbowCycleVarInt[0]) & 255);
      leds[parameters.nbLeds - 1 - i].setRGB(*c, *(c+1), *(c+2));
    }
    parameters.rainbowCycleVarInt[0]++;
    parameters.rainbowCycleLastActivate = millisecs;
  }
}

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

void Animations::fadeInAndOut(CRGB* leds, animParamRef& parameters){//, int randomColors){
  float r, g, b;
  // FADE IN
  //for(int i = 0; i <= 255; i++) {
  if(millisecs - parameters.fadeInAndOutLastActivate > parameters.fadeInAndOutParamInt[3]){
    r = (parameters.fadeInAndOutFadeAmount/256.0)*parameters.fadeInAndOutParamInt[0];
    g = (parameters.fadeInAndOutFadeAmount/256.0)*parameters.fadeInAndOutParamInt[1];
    b = (parameters.fadeInAndOutFadeAmount/256.0)*parameters.fadeInAndOutParamInt[2];
    fill_solid(leds, parameters.nbLeds, CRGB(r, g, b));
    if(parameters.fadeInAndOutFadingIn){
      parameters.fadeInAndOutFadeAmount++;
    }
    else{
      parameters.fadeInAndOutFadeAmount--;
    }
    if(parameters.fadeInAndOutFadeAmount == 25){
      parameters.fadeInAndOutFadingIn = false;
    }
    else if(parameters.fadeInAndOutFadeAmount == 0){
      parameters.fadeInAndOutFadingIn = true;
    }
    parameters.fadeInAndOutLastActivate = millisecs;
  }
}

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

void Animations::sparkle(CRGB* leds, animParamRef& parameters) {
  EVERY_N_MILLISECONDS(parameters.sparkleParamInt[3]){
    leds[parameters.sparkleVarInt[1]].setRGB(0, 0, 0);
    parameters.sparkleVarInt[1] = random(parameters.nbLeds);
    leds[parameters.sparkleVarInt[1]].setRGB(parameters.sparkleParamInt[1], parameters.sparkleParamInt[0], parameters.sparkleParamInt[2]);
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
void Animations::Fire(CRGB* leds, animParamRef& parameters) {
  int cooldown;
  // Cool down each cell a little
  if(millisecs - parameters.fireLastActivate > parameters.fireParamInt[2]){
    for(int i = 0; i < parameters.nbLeds; i++) {
      cooldown = random(0, ((parameters.fireParamInt[0] * 10) / parameters.nbLeds) + 2);
      if(cooldown > parameters.fireHeat[i]) {
        parameters.fireHeat[i] = 0;
      }
      else {
        parameters.fireHeat[i] = parameters.fireHeat[i] - cooldown;
      }
    }
    // Heat from each cell drifts up and diffuses slightly
    for(int k = (parameters.nbLeds - 1); k >= 2; k--) {
      parameters.fireHeat[k] = (parameters.fireHeat[k - 1] + parameters.fireHeat[k - 2] + parameters.fireHeat[k - 2]) / 3;
    }
    // Randomly ignite new Sparks near bottom of the flame
    if(random(255) < parameters.fireParamInt[1]) {
      int y = random(7);
      parameters.fireHeat[y] = parameters.fireHeat[y] + random(160, 255);
    }
    // Convert heat to LED colors
    for(int j = 0; j < parameters.nbLeds; j++) {
      setPixelHeatColor(leds, j, parameters.fireHeat[j], parameters.fireParamFloat[0]);
    }
    parameters.fireLastActivate = millisecs;
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
void Animations::shootingStar(CRGB* leds, animParamRef& parameters){
  if(millisecs - parameters.shootingStarLastActivate >= parameters.shootingStarParamInt[6]){
    if (millisecs - parameters.shootingStarLastStar >= parameters.shootingStarParamInt[5]) {
      parameters.shootingStarLastStar = millisecs;         // Save the last time the LEDs were updated
      //shootingStarCount = 0;
      parameters.shootingStarPosCounter = 0;                              // Reset the count to 0 after each interval
    }
    if (parameters.shootingStarParamInt[6] == -1) {        // Reverse direction option for LEDs
      if (parameters.shootingStarPosCounter < parameters.nbLeds) {
        leds[parameters.nbLeds - (parameters.shootingStarPosCounter % (parameters.nbLeds+1))].setRGB(parameters.shootingStarParamInt[0], parameters.shootingStarParamInt[1], parameters.shootingStarParamInt[2]);    // Set LEDs with the color value
        parameters.shootingStarPosCounter++;
      }
    }
    else {
      if (parameters.shootingStarPosCounter < parameters.nbLeds) {     // Forward direction option for LEDs
        leds[parameters.shootingStarPosCounter % (parameters.nbLeds+1)].setRGB(parameters.shootingStarParamInt[0], parameters.shootingStarParamInt[1], parameters.shootingStarParamInt[2]);    // Set LEDs with the color value
        parameters.shootingStarPosCounter++;
      }
    }
    fadeToBlackBy(leds, parameters.nbLeds, parameters.shootingStarParamInt[3]);                 // Fade the tail LEDs to black
  }
  parameters.shootingStarLastActivate = millisecs;
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
void Animations::twinklePixels(CRGB* leds, animParamRef& parameters) {
  if(millisecs - parameters.twinklePixelsLastActivate > parameters.twinklePixelsParamInt[4]){
    for (int i = 0; i < parameters.nbLeds; i++) {
      // Draw twinkling pixels
      if (random(parameters.twinklePixelsParamInt[2]) < 2) {     // Chance for pixel to twinkle
        uint8_t intensity = random(100, 255);     // Configure random intensity
        //CHSV(Color, ColorSaturation, intensity);     // Configure color with max saturation and variable value (intensity)
        leds[i].setHSV(parameters.twinklePixelsParamInt[0], parameters.twinklePixelsParamInt[1], intensity);    // Set the pixel color
      }
      // Fade LEDs
      if (leds[i].r > 0 || leds[i].g > 0 || leds[i].b > 0) {    // Check if pixel is lit
        leds[i].fadeToBlackBy(parameters.twinklePixelsParamInt[3]);    // Fade pixel to black
      }
    }
    parameters.twinklePixelsLastActivate = millisecs;
  }
}

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

void Animations::strobe(CRGB* leds, animParamRef& parameters){
  if(millisecs - parameters.strobeLastOn > parameters.strobeParamInt[0] + parameters.strobeParamInt[1]){
    parameters.strobeLastOn = millisecs;
    fill_solid(leds, parameters.nbLeds, CRGB(parameters.strobeParamInt[2], parameters.strobeParamInt[3], parameters.strobeParamInt[4]));
  }
  else if (millisecs - parameters.strobeLastOff > parameters.strobeParamInt[0]) {
    parameters.strobeLastOff = millisecs;
    FastLED.clear();
  }
}

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

void Animations::zip(CRGB* leds, animParamRef& parameters){
  //static unsigned long zip_animation_prev_mills = 0;
  //static int zip_animation_pos_counter = 0;
  int startPos, endPos;
  if(millisecs - parameters.zipLastActivate > parameters.zipParamUnsignedLong[0]){
    // This is the start of the strip
    startPos = parameters.zipPosCounter + parameters.zipParamInt[1];
    if(startPos <= parameters.nbLeds && startPos < parameters.zipParamInt[2]){
      leds[startPos].setRGB(parameters.zipParamInt[4], parameters.zipParamInt[5], parameters.zipParamInt[6]);
    }
    // This is the end of the strip
    endPos = parameters.zipPosCounter + parameters.zipParamInt[1] - parameters.zipParamInt[0];
    if(endPos >= 0 && endPos <= parameters.nbLeds){
      leds[endPos].setRGB(0,0,0);
    }
    //FastLED.show();
    parameters.zipPosCounter = ++parameters.zipPosCounter % (parameters.zipParamInt[2] - parameters.zipParamInt[1] + parameters.zipParamInt[0]);
    parameters.zipLastActivate = millisecs;
  }
} 

void Animations::flashToBeat(CRGB* leds, animParamRef& parameters){
  if(detectedKick){
    //Serial.print("Beat type: ");Serial.println(beatType);
    fill_solid(leds, parameters.nbLeds, CRGB(parameters.flashToBeatParamInt[1], parameters.flashToBeatParamInt[0], parameters.flashToBeatParamInt[2]));
  }
  if(millisecs - parameters.flashToBeatLastActivate > parameters.flashToBeatParamInt[3]){
    fill_solid(leds, parameters.nbLeds, CRGB(0, 0, 0));
    parameters.flashToBeatLastActivate = millis();//millisecs;
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
