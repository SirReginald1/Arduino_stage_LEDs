/**
  * ESP32 Animations
*/

#include "Arduino.h"
#include <FastLED.h>
#include "Globals.h"
#include "Com_interface.h"


extern float* timings; 
extern int timingsLength;

/**Struct that contains all the references to the aniamtion parameters*/
struct animParamRef{
  int rainbowCycleParamInt[1] = {2000}; // {int delay}

  int fadeInAndOutParamInt[3] = {255, 255, 255}; // {int red, int green, int blue}

  int sparkleParamInt[4] = {200, 0, 100, 0}; // {int red, int green, int blue, int delay}

  int fireParamInt[3] = {50, 50, 0}; // {int flame_height, int sparks, int delay}
  float fireParamFloat[1] = {1.}; // {float fire_intensity}

  int shootingStarParamInt[7] = {150, 0, 150, 20, 10, 2000, 1}; // {int R, int G, int B, int tail_length, int delay_duration, int interval, int direction}

  int twinklePixelsParamInt[5] = {200, 50, 50, 20, 100}; // {int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration}

  int strobeParamInt[5] = {20, 55, 255, 255, 255}; // {int time_on, int time_off, int R, int G, int B}

  int zipParamInt[7] = {2, 10, NUM_LEDS-5, 0, 0, 0, 255}; // {int size, int start, int end, int delay, int R, int G, int B}
  unsigned long zipParamUnsignedLong[1] = {20}; // {unsigned long speed, unsigned long current_time}
};


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

    static void runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefArray[NB_ARRAYS], unsigned long millisecs);

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

    static void rainbowCycle(CRGB* leds, int DelayDuration, int millisecs);

    static void rainbowCycle(CRGB leds[NB_ARRAYS][NUM_LEDS], int DelayDuration, int millisecs);

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

    static void fadeInAndOut(CRGB* leds, int red, int green, int blue);

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

    static void sparkle(CRGB* leds, int red, int green, int blue, int delayDuration, unsigned long millisecs);

    static void sparkle(CRGB leds[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefs[NB_ARRAYS], unsigned long millisecs);

// #########################################################################
// ############################ Fire animation #############################
// #########################################################################

    // FlameHeight - Use larger value for shorter flames, default=50.
    // Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
    // DelayDuration - Use larger value for slower flame speed, default=10.
    static void Fire(CRGB* leds, int FlameHeight, int Sparks, int DelayDuration, float intensity);

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
    static void shootingStar(CRGB* leds, int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction, unsigned long currentMillis);

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
    static void twinklePixels(CRGB* leds, int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration);

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

    static void strobe(CRGB* leds, int time_on, int time_off, CRGB color);

    static void strobe(CRGB leds[NB_ARRAYS][NUM_LEDS], int time_on, int time_off, CRGB color);

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

    static void zip(CRGB* leds, int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);

    static void zip(CRGB leds[NB_ARRAYS][NUM_LEDS], int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color);

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

    static void flashToBeat(CRGB leds[NB_ARRAYS][NUM_LEDS], float* timings, int* timingsLength, unsigned long current_time, CRGB color);

    static void stopFlashToBeat();

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

bool Animations::flashToBeatGo = false;

bool Animations::flashToBeatStarted = false;

/**
* Function that must be called by any action that interupts the animation.
*/
void Animations::stopFlashToBeat(){
  Animations::flashToBeatGo = false;
  Animations::flashToBeatStarted = false;
}

// ###########################################################################################################################
// ############################################### STATIC DEFINITIONS ########################################################
// ###########################################################################################################################

void Animations::runAnimations(CRGB ledArrays[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefArray[NB_ARRAYS], unsigned long millisecs){
  int i;
  // Run animations with com interface
  #ifdef USE_INTERFACE
    switch (ComInterface::getAnimation()) {
  #endif
  #ifndef USE_INTERFACE
    switch (animation) {
  #endif
    case 1:
      for(i=0;i<NB_ARRAYS;i++){
        //Serial.print("Val param: ");Serial.println(animParamRefArray[i].rainbowCycleParamInt[0]);
        Animations::rainbowCycle(ledArrays[i], animParamRefArray[i].rainbowCycleParamInt[0], millisecs);
      }
      //Test::test();
      break;
    case 2:
      for(i=0;i<NB_ARRAYS;i++){
        Animations::fadeInAndOut(ledArrays[i], random(255), random(255), random(255));
      }
      //Animations::fadeInAndOut(ledArrays[1], random(&animations[1][0]), random(&animations[1][1]), random(&animations[1][2]));
      break;
    case 3:
    /*
      for(i=0;i<NB_ARRAYS;i++){
        //Serial.print("i: ");Serial.println(i);
        
        Animations::sparkle(ledArrays[i], 
                            animParamRefArray[i].sparkleParamInt[1], // Red and Green are reversed in FastLED
                            animParamRefArray[i].sparkleParamInt[0], 
                            animParamRefArray[i].sparkleParamInt[2], 
                            animParamRefArray[i].sparkleParamInt[3], 
                            millisecs);                    
      }*/
      //FastLED.clear(true);
      Animations::sparkle(ledArrays,  animParamRefArray, millisecs);
      //FastLED.show();
      break;
    case 4:
      for(i=0;i<NB_ARRAYS;i++){
        Animations::Fire(ledArrays[i], 
                        animParamRefArray[i].fireParamInt[0], 
                        animParamRefArray[i].fireParamInt[1], 
                        animParamRefArray[i].fireParamInt[2], 
                        animParamRefArray[i].fireParamFloat[0]);
      }
      break;
    case 5:
      for(i=0;i<NB_ARRAYS;i++){
        Animations::shootingStar(ledArrays[i], 
                                animParamRefArray[i].shootingStarParamInt[1], 
                                animParamRefArray[i].shootingStarParamInt[0], 
                                animParamRefArray[i].shootingStarParamInt[2], 
                                animParamRefArray[i].shootingStarParamInt[3], 
                                animParamRefArray[i].shootingStarParamInt[4], 
                                animParamRefArray[i].shootingStarParamInt[5], 
                                animParamRefArray[i].shootingStarParamInt[6], 
                                millisecs);
      }
      break;
    case 6:
      for(i=0;i<NB_ARRAYS;i++){
        Animations::twinklePixels(ledArrays[i], 
                                  animParamRefArray[i].twinklePixelsParamInt[0], 
                                  animParamRefArray[i].twinklePixelsParamInt[1], 
                                  animParamRefArray[i].twinklePixelsParamInt[2], 
                                  animParamRefArray[i].twinklePixelsParamInt[3], 
                                  animParamRefArray[i].twinklePixelsParamInt[4]);
      }
      break;
    case 7:
      //#ifdef USE_MIC
      //  volum_bar_animation(led_arrays[0], millisecs, NUM_LEDS);
      //#endif
        Animations::flashToBeat(ledArrays, 
                                timings, 
                                &timingsLength, 
                                millisecs, 
                                CRGB(0 ,200, 100));
      break;
    case 8:
      Animations::strobe(ledArrays, 
                        animParamRefArray[i].strobeParamInt[0], 
                        animParamRefArray[i].strobeParamInt[1], 
                        CRGB(animParamRefArray[i].strobeParamInt[3], 
                            animParamRefArray[i].strobeParamInt[2], 
                            animParamRefArray[i].strobeParamInt[4])
                        );
      break;
    case 9:
      for(i=0;i<NB_ARRAYS;i++){
        Animations::zip(ledArrays, 
                        animParamRefArray[i].zipParamInt[0], 
                        animParamRefArray[i].zipParamInt[1], 
                        animParamRefArray[i].zipParamInt[2], 
                        animParamRefArray[i].zipParamInt[3], 
                        animParamRefArray[i].zipParamUnsignedLong[0], 
                        millisecs, 
                        CRGB(animParamRefArray[i].zipParamInt[4],
                            animParamRefArray[i].zipParamInt[5],
                            animParamRefArray[i].zipParamInt[6])
                        );
      }
      break;
    default:
      //Serial.println("Animation code not recognized!");
      #ifdef USE_INTERFACE
        ComInterface::setAnimation(1);
        // This simbole means that it has not recognised the animation and the interface must therefore resend it send!
        // TODO: Check that this is stille needed for the esp32 as serial seems to work much better on that board.
        Serial.print("#!@");
      #endif
      #ifndef USE_INTERFACE
        extern animation = 1;
      #endif
  }
  FastLED.show();
}


// #########################################################################
// ##################### Animation Parameter References ####################
// #########################################################################

    int* Animations::getParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case 1:
          return animationStruct[arrayIdx].rainbowCycleParamInt;
          break;
        case 2:
          return animationStruct[arrayIdx].fadeInAndOutParamInt;
          break;
        case 3:
          return animationStruct[arrayIdx].sparkleParamInt;
          break;
        case 4:
          return animationStruct[arrayIdx].fireParamInt;
          break;
        case 5:
          return animationStruct[arrayIdx].shootingStarParamInt;
          break;
        case 6:
          return animationStruct[arrayIdx].twinklePixelsParamInt;
          break;
        /*case 7:
          #ifdef USE_MIC
            volum_bar_animation(led_arrays[0], millisecs, NUM_LEDS);
          #endif
          #ifndef USE_MIC
            Animations::strobe(led_arrays, 20, 55, CRGB(255, 255, 255));
          #endif
          break;*/
        case 8:
          return animationStruct[arrayIdx].strobeParamInt;
          break;
        case 9:
          return animationStruct[arrayIdx].zipParamInt;
          break;
      }
    }

    float* Animations::getParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case 4:
          return animationStruct[arrayIdx].fireParamFloat;
          break;
      }
    }

    unsigned long* Animations::getParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode){
      switch (animationCode) {
        case 9:
          return animationStruct[arrayIdx].zipParamUnsignedLong;
          break;
      }
    }

    void Animations::setParametersInt(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, int paramValue){
      switch (animationCode){
        case 1:
          animationStruct[arrayIdx].rainbowCycleParamInt[paramIdx] = paramValue;
          break;
        case 2:
          animationStruct[arrayIdx].fadeInAndOutParamInt[paramIdx] = paramValue;
          break;
        case 3:
          animationStruct[arrayIdx].sparkleParamInt[paramIdx] = paramValue;
          break;
        case 4:
          animationStruct[arrayIdx].fireParamInt[paramIdx] = paramValue;
          break;
        case 5:
          animationStruct[arrayIdx].shootingStarParamInt[paramIdx] = paramValue;
          break;
        case 6:
          animationStruct[arrayIdx].twinklePixelsParamInt[paramIdx] = paramValue;
          break;
        /*case 7:
          #ifdef USE_MIC
            
          #endif
          #ifndef USE_MIC
            
          #endif
          break;*/
        case 8:
          animationStruct[arrayIdx].strobeParamInt[paramIdx] = paramValue;
          break;
        case 9:
          animationStruct[arrayIdx].zipParamInt[paramIdx] = paramValue;
          break;
      }
    }

    void Animations::setParametersFloat(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, float paramValue){
      switch (animationCode){
        case 4:
          animationStruct[arrayIdx].fireParamFloat[paramIdx] = paramValue;
          break;
      }
    }

    void Animations::setParametersUnsignedLong(animParamRef animationStruct[NB_ARRAYS], int arrayIdx, int animationCode, int paramIdx, unsigned long paramValue){
      switch (animationCode){
        case 9:
          animationStruct[arrayIdx].zipParamUnsignedLong[paramIdx] = paramValue;
          break;
      }
    }

// #########################################################################
// ####################### Rainbow Cycle animation #########################
// #########################################################################

void Animations::rainbowCycle(CRGB* leds, int DelayDuration, int millisecs){
  static unsigned long rainbowCyclePreviousMillis = 0;
  static uint64_t rainbowCycleAnimCount = 0;
  byte *c;
  uint16_t i, j;
  if(millisecs - rainbowCyclePreviousMillis >= DelayDuration){
    if(rainbowCycleAnimCount >256){
      rainbowCycleAnimCount = 0;
    }
    //for(j=0; j < 256; j++) {
      for(i=0; i < NUM_LEDS; i++) { // This makes sure all LEDs on strip are updated
        c = Wheel(((i * 256 / NUM_LEDS) + /*j*/ rainbowCycleAnimCount) & 255);
        leds[NUM_LEDS - 1 - i].setRGB(*c, *(c+1), *(c+2));
      }
      FastLED.show();
      //delay(DelayDuration);
      rainbowCycleAnimCount++;
    //}
  }
}

/**
  * Rainbow Circle animation that is ment to run with an array of LED arrays. All arrays ars synced.
*/
void Animations::rainbowCycle(CRGB leds[NB_ARRAYS][NUM_LEDS], int DelayDuration, int millisecs) {
  static unsigned long rainbowCyclePreviousMillis;
  static uint16_t rainbowCycleAnimCount;
  byte *c;
  uint16_t i, j;
  if(millisecs - rainbowCyclePreviousMillis >= DelayDuration){
    if(rainbowCycleAnimCount >256){
      rainbowCycleAnimCount = 0;
    }
    for(i=0; i < NUM_LEDS; i++) { // This makes sure all LEDs on strip are updated
      c = Wheel(((i * 256 / NUM_LEDS) + /*j*/ rainbowCycleAnimCount) & 255);
      for(j=0;j<NB_ARRAYS;j++){
        leds[j][NUM_LEDS - 1 - i].setRGB(*c, *(c+1), *(c+2));
      }
    }
    FastLED.show();
    rainbowCycleAnimCount++;
  }
}

// #########################################################################
// ############################ Fade animation #############################
// #########################################################################

void Animations::fadeInAndOut(CRGB* leds, int red, int green, int blue){//, int randomColors){
  float r, g, b;
  // FADE IN
  for(int i = 0; i <= 255; i++) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }
  // FADE OUT
  for(int i = 255; i >= 0; i--) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    for(int i=0; i<NUM_LEDS;i++){
      leds[i].setRGB(r, g, b);// = CRGB(r,g,b);
    }
    //fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(2);
  }
}

//   #########################################################################
//   ####################### Sparkle animation ###############################
//   #########################################################################

void Animations::sparkle(CRGB* leds, int red, int green, int blue, int delayDuration, unsigned long millisecs) {
  static unsigned long SparklePreviousMillis = 0;
  if(millisecs - SparklePreviousMillis >= delayDuration){
    int pixel = random(NUM_LEDS);
    leds[pixel].setRGB(red, green, blue);
    FastLED.show();
    leds[pixel].setRGB(0, 0, 0);
  }
} 

void Animations::sparkle(CRGB leds[NB_ARRAYS][NUM_LEDS], animParamRef animParamRefs[NB_ARRAYS], unsigned long millisecs) {
  static unsigned long SparklePreviousMillis = 0;
  static int pixel[NB_ARRAYS];
  for(int i=0;i<NB_ARRAYS;i++){
    if(millisecs - SparklePreviousMillis >= animParamRefs[i].sparkleParamInt[3]){
      leds[i][pixel[i]].setRGB(0, 0, 0);
      pixel[i] = random(NUM_LEDS);
      leds[i][pixel[i]].setRGB(animParamRefs[i].sparkleParamInt[1], animParamRefs[i].sparkleParamInt[0], animParamRefs[i].sparkleParamInt[2]);
    }
  }
  //if(millisecs - SparklePreviousMillis >= animParamRefs[i].sparkleParamInt[3]){
  //FastLED.show();
  //}
  /*for(int i=0;i<NB_ARRAYS;i++){
    leds[i][pixel[i]].setRGB(0, 0, 0);
  }*/
    SparklePreviousMillis = millisecs;
} 

// #########################################################################
// ############################ Fire animation #############################
// #########################################################################

// FlameHeight - Use larger value for shorter flames, default=50.
// Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255), default=100.
// DelayDuration - Use larger value for slower flame speed, default=10.

void Animations::Fire(CRGB* leds, int FlameHeight, int Sparks, int DelayDuration, float intensity) {
  static byte heat[NUM_LEDS];
  int cooldown;
  // Cool down each cell a little
  for(int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((FlameHeight * 10) / NUM_LEDS) + 2);
    if(cooldown > heat[i]) {
      heat[i] = 0;
    }
    else {
      heat[i] = heat[i] - cooldown;
    }
  }
  // Heat from each cell drifts up and diffuses slightly
  for(int k = (NUM_LEDS - 1); k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Randomly ignite new Sparks near bottom of the flame
  if(random(255) < Sparks) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }
  // Convert heat to LED colors
  for(int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(leds, j, heat[j], intensity);
  }
  FastLED.show();
  delay(DelayDuration);
}

// #########################################################################
// ######################## Shooting Star animation ########################
// #########################################################################

/** 
 *  ========== Shooting Star Animation ==========
 *  red, green, blue - Choose a color with RGB values (0 to 255).
 *  tail_length - A larger value results in shorter tail.
 *  delay_duration - A larger value results in slower movement speed.
 *  interval - Time interval between new shooting stars (in milliseconds).
*/
void Animations::shootingStar(CRGB* leds, int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction, unsigned long currentMillis){
  static unsigned long shootingStarPreviousMillis = 0;           // Stores last time LEDs were updated
  static int shootingStarCount = 0;                              // Stores count for incrementing up to the NUM_LEDs
  /*
   * red - 0 to 255 red color value
   * green - 0 to 255 green color value
   * blue - 0 to 255 blue color value
   * tail_length - value which represents number of pixels used in the tail following the shooting star
   * delay_duration - value to set animation speed. Higher value results in slower animation speed.
   * interval - time between each shooting star (in miliseconds)
   * direction - value which changes the way that the pixels travel (uses -1 for reverse, any other number for forward)
  */
  //unsigned long currentMillis = millis();   // Get the time
  if (currentMillis - shootingStarPreviousMillis >= interval) {
    shootingStarPreviousMillis = currentMillis;         // Save the last time the LEDs were updated
    shootingStarCount = 0;                              // Reset the count to 0 after each interval
  }
  if (direction == -1) {        // Reverse direction option for LEDs
    if (shootingStarCount < NUM_LEDS) {
      leds[NUM_LEDS - (shootingStarCount % (NUM_LEDS+1))].setRGB(red, green, blue);    // Set LEDs with the color value
      shootingStarCount++;
    }
  }
  else {
    if (shootingStarCount < NUM_LEDS) {     // Forward direction option for LEDs
      leds[shootingStarCount % (NUM_LEDS+1)].setRGB(red, green, blue);    // Set LEDs with the color value
      shootingStarCount++;
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, tail_length);                 // Fade the tail LEDs to black
  FastLED.show();
  delay(delay_duration);                                      // Delay to set the speed of the animation
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
void Animations::twinklePixels(CRGB* leds, int Color, int ColorSaturation, int PixelVolume, int FadeAmount, int DelayDuration) {
  for (int i = 0; i < NUM_LEDS; i++) {
    // Draw twinkling pixels
    if (random(PixelVolume) < 2) {     // Chance for pixel to twinkle
      uint8_t intensity = random(100, 255);     // Configure random intensity
      CRGB set_color = CHSV(Color, ColorSaturation, intensity);     // Configure color with max saturation and variable value (intensity)
      leds[i] = set_color;    // Set the pixel color
    }
    // Fade LEDs
    if (leds[i].r > 0 || leds[i].g > 0 || leds[i].b > 0) {    // Check if pixel is lit
      leds[i].fadeToBlackBy(FadeAmount);    // Fade pixel to black
    }
  }
  FastLED.show();
  delay(DelayDuration);
}

// #########################################################################
// ########################### Strobe animation ############################
// #########################################################################

void Animations::strobe(CRGB* leds, int time_on, int time_off, CRGB color){
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
  delay(time_on);
  FastLED.clear();
  FastLED.show();
  delay(time_off);
}

void Animations::strobe(CRGB leds[NB_ARRAYS][NUM_LEDS], int time_on, int time_off, CRGB color){
  for(int i=0;i<NB_ARRAYS;i++){
    fill_solid(leds[i], NUM_LEDS, color);
  }
  FastLED.show();
  delay(time_on);
  FastLED.clear();
  FastLED.show();
  delay(time_off);
}

// ############################################################################
// ############################## Zip animation ###############################
// ############################################################################

void Animations::zip(CRGB* leds, int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color){
  static unsigned long zip_animation_prev_mills = 0;
  static int zip_animation_pos_counter = 0;
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

void Animations::zip(CRGB leds[NB_ARRAYS][NUM_LEDS], int size, int start, int end, int delay, unsigned long speed, unsigned long current_time, CRGB color){
  static unsigned long zip_animation_prev_mills = 0;
  static int zip_animation_pos_counter = 0;
  int start_pos, end_pos;
  if(current_time - zip_animation_prev_mills > speed){
    // This is the start of the strip
    start_pos = zip_animation_pos_counter + start;
    if(start_pos <= NUM_LEDS && start_pos < end){
      for(int i=0;i<NB_ARRAYS;i++){
        leds[i][start_pos] = color;
      }
    }
    // This is the end of the strip
    end_pos = zip_animation_pos_counter + start - size;
    if(end_pos >= 0 && end_pos <= NUM_LEDS){
      for(int i=0;i<NB_ARRAYS;i++){
        leds[i][end_pos] = CRGB::Black;
      }
    }
    FastLED.show();
    zip_animation_pos_counter = ++zip_animation_pos_counter % (end - start + size);
    zip_animation_prev_mills = current_time;
  }
}

// #############################################################################################################
// ############################################ BEAT ANIMATIONS ################################################
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
void Animations::flashToBeat(CRGB leds[NB_ARRAYS][NUM_LEDS], float* timings, int* timingsLength, unsigned long current_time, CRGB color){
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
