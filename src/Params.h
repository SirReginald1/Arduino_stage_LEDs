#ifndef Params_h
#define Params_h

#include "Arduino.h"
#include "Globals.h"

#define PARAM_TYPE_CODE_INT 0
#define PARAM_TYPE_CODE_FLOAT 1
#define PARAM_TYPE_CODE_UNSIGNED_LONG 2

/* The number of parameter type arrays. Used to find max array sizes. 
    !!!!! If you add parameter types remember to modify the getParamArraySizes function !!!!!!!!!!!!!!! */
const uint8_t nbParamType = 8;
/* Value used to automaticaly set the array ID value for each AnimParams. Incremented in the constructor. */
static uint8_t nbArraysInitialized = 0;
/* Value used to set stack adjustable parameter integer reference array size. */
const uint8_t stackAdjustParamIntArraySize = 7;
/* Value used to set stack adjustable parameter float reference array size. */
const uint8_t stackAdjustParamFloatArraySize = 1;
/* Value used to set stack adjustable parameter unsigned long reference array size. */
const uint8_t stackAdjustParamLongArraySize = 1;
/* Value used to set stack parameter byte reference array size. */
const uint8_t stackParamByteArraySize = 122;
/* Value used to set stack parameter boolean reference array size. */
const uint8_t stackParamBoolArraySize = 1;
/* Value used to set stack parameter integer reference array size. */
const uint8_t stackParamIntArraySize = 1;
/* Value used to set stack parameter float reference array size. */
const uint8_t stackParamFloatArraySize = 0;
/* Value used to set stack parameter unsigned long reference array size. */
const uint8_t stackParamLongArraySize = 2;

/* The size of  */

/**
 * The parent class used to store all parameter objects for a given animation.
 */
class Parameters{
    public: // TODO: Turn all nbParam variables into an array
    unsigned long timeLastActivate = 0; /* The time at which this animation was last triggered. */
    /* Array the code for the type of the parameters of the animation. Is in the same order as the parameters in 
    parameter object decleration. */
    const int* paramPosType;
    /* Value indicating werther the animation loaded into this stack object has position data. */
    bool hasPosData; // !!!!!!!!!!!!!! Make this a constant !!!!!!!!!!!!!!!!!!!!!
    // ########################### adjustable parameters ###############################
    const uint8_t nbAdjustParamInt; /* The number of adjustable integer parameters the animation has. */
    const uint8_t nbAdjustParamFloat; /* The number of adjustable float parameters the animation has. */
    const uint8_t nbAdjustParamLong; /* The number of adjustable unsigned long parameters the animation has. */
    int* intAdjustParams; /* The array containing the adjustable integer parameters. */
    float* floatAdjustParams; /* The array containing the adjustable float parameters. */
    unsigned long* uLongAdjustParams; /* The array containing the adjustable unsigned long parameters. */
    // ######################### non adjustable parameters #############################
    const uint8_t nbParamByte; /* The number of non adjustable byte parameters the animation has. */
    const uint8_t nbParamBool; /* The number of non adjustable boolean parameters the animation has. */
    const uint8_t nbParamInt; /* The number of non adjustable integer parameters the animation has. */
    const uint8_t nbParamFloat; /* The number of non adjustable float parameters the animation has. */
    const uint8_t nbParamLong; /* The number of non adjustable unsigned long parameters the animation has. */
    byte* byteParams; /* The array containing the non adjustable byte parameters. */
    bool* boolParams; /* The array containing the non adjustable bool parameters. */
    int* intParams; /* The array containing the non adjustable int parameters. */
    float* floatParams; /* The array containing the non adjustable float parameters. */
    unsigned long* uLongParams; /* The array containing the non adjustable unsigned long parameters. */
    // ########################## Default function parameters ##################################
    static const bool defaultPrintSelect[nbParamType]; /* Default parameter for parameter selection in printParameters function */

    /**
     * Prints all the parameters.
     * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
     */
    void printParameters(const bool* printSelect = defaultPrintSelect);

    /**
     * Constuctor for the generic object that holds all the animation parameters.
     * @param nbAdjustParamInt The number of adjustable integer parameters the animation has.
     * @param nbAdjustParamFloat The number of adjustable float parameters the animation has.
     * @param nbAdjustParamLong The number of adjustable unsigned long parameters the animation has.
     * @param nbParamByte The number of non adjustable byte parameters the animation has.
     * @param nbParamBool The number of non adjustable boolean parameters the animation has.
     * @param nbParamInt The number of non adjustable integer parameters the animation has.
     * @param nbParamFloat The number of non adjustable floats parameters the animation has.
     * @param nbParamLong The number of non adjustable unsigned long parameters the animation has.
     */
    Parameters(uint8_t nbAdjustParamInt = 0,
               uint8_t nbAdjustParamFloat = 0,
               uint8_t nbAdjustParamLong = 0,
               uint8_t nbParamByte = 0,
               uint8_t nbParamBool = 0,
               uint8_t nbParamInt = 0, 
               uint8_t nbParamFloat = 0, 
               uint8_t nbParamLong = 0);

    Parameters(uint8_t paramTypeSizeList[nbParamType]);

    ~ Parameters();
    // Maby add these methods later: https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
};

/**
 * Object containing parameters for Rainbow Cycle animation.
 * Int Parameters: {int delay}
 * @param arrayId The identifier number for the array this parameter object is attached to.
 * @param nbLeds The number of LEDs the array the parameter object is attached to has.
 * @param speed The speed of the animation.
 */
class RainbowCycleParams : public Parameters{
public:

    /**
    * Constructor for Rainbow Cycle animation.
    * Int Parameters: {int delay}
    * Int parameters: {animPos}
    * @param speed The speed of the animation.
    */
    RainbowCycleParams(int speed = 0);

    ~ RainbowCycleParams();
};

/**
 * Object containing parameters for fade in and out animation.
 * Int Parameters: {red, green, int blue, int speed}
 * Int parameters: {animPos}
 * @param red Amount of red.
 * @param green Amount of green.
 * @param blue Amount of blue.
 * @param speed The speed of the animation.
 */
class FadeInAndOutParams : public Parameters{
    public:
    //bool fadeInAndOutFadingIn = true;
        /**
        * Consturctor parameters for fade in and out animation.
        * Int Parameters: {red, green, int blue, int speed}
        * Int parameters: {animPos}
        * @param red Amount of red.
        * @param green Amount of green.
        * @param blue Amount of blue.
        * @param speed The speed of the animation.
        */
        FadeInAndOutParams(int red = 255, 
                           int green = 255, 
                           int blue = 255, 
                           unsigned long  speed = 100UL);
    
        ~ FadeInAndOutParams();
};

/**
 * Object containing parameters for sparkle animation.
 * Int Parameters: {red, green, int blue, int speed}
 * Int parameters: {animPos}
 * @param red Amount of red.
 * @param green Amount of green.
 * @param blue Amount of blue.
 * @param delay The speed of the animation.
 */
class SparkleParams : public Parameters{
    public:
    //int sparkleVarInt = 0;
        /**
        * Consturctor parameters sparkle animation.
        * Int Adjust Parameters: {red, green, blue, delay}
        * Int parameters: {animPos}
        * @param red Amount of red.
        * @param green Amount of green.
        * @param blue Amount of blue.
        * @param delay The speed of the animation.
        */
       SparkleParams(int red = 200, 
                     int green = 0, 
                     int blue = 100, 
                     int delay = 0);
    
        ~ SparkleParams();
};

/**
 * Object containing parameters for the fire animation.
 * Int Parameters: {flame_height, sparks, delay}
 * Int parameters: {animPos}
 * Float Parameters : {fire_intensity}
 * @param flameHeight The height of the flames.
 * @param sparks The amount of sparks.
 * @param delay The speed of the animation.
 * @param fireIntensity How hot the fire burns (What colors to put on what part of the flame).
 */
class FireParams : public Parameters{
    public:
    //byte* fireHeat;
        /**
        * Consturctor parameters fire animation.
        * Int Parameters: {flame_height, sparks, delay}
        * Int parameters: {animPos}
        * Float Parameters : {fire_intensity}
        * @param nbLeds The number of leds in the array. Used to adjust the byte array of flame heat.
        * @param flameHeight The height of the flames.
        * @param sparks The amount of sparks.
        * @param delay The speed of the animation.
        * @param fireIntensity How hot the fire burns (What colors to put on what part of the flame).
        */
       FireParams(int nbLeds = NUM_LEDS, 
                  int flameHeight = 50, 
                  int sparks = 50, 
                  int delay = 25, 
                  float fireIntensity = 1.);
    
        ~ FireParams();
};

/**
 * Object containing parameters for the shooting star animation.
 * Int Parameters: {red, green, blue, tail_length, delay_duration, interval, direction}
 * Int parameters: {animPos}
 * @param red The amount of red.
 * @param green The amount of green.
 * @param blue The amount of blue.
 * @param tailLength The length of the tail of the shooting star. (The fade strength)
 * @param delayDuration The speed of the animation.
 * @param interval The interval between shooting stars.
 * @param direction Which direction the shooting stars are going.
 */
class ShootingStarParams : public Parameters{
    public:
    //unsigned long shootingStarLastStar = 0; // The last time a shooting star started
        /**
        * Consturctor parameters shooting star animation.
        * Int Parameters: {red, green, blue, tail_length, delay_duration, interval, direction}
        * Int parameters: {animPos}
        * @param red The amount of red.
        * @param green The amount of green.
        * @param blue The amount of blue.
        * @param tailLength The length of the tail of the shooting star. (The fade strength)
        * @param delayDuration The speed of the animation.
        * @param interval The interval between shooting stars.
        * @param direction Which direction the shooting stars are going.
        */
       ShootingStarParams(int red = 150, 
                          int green = 0, 
                          int blue = 150, 
                          int tailLength = 20, 
                          unsigned long delayDuration = 10UL, 
                          unsigned long interval = 2000UL, 
                          int direction = 1);
    
        ~ ShootingStarParams();
};

/**
 * Object containing parameters for the twinkle pixels animation.
 * Int Parameters: {hue, saturation, pixelVolume, fadeAmount, delayDuration}
 * Int parameters: {animPos}
 * @param hue The color hue.
 * @param saturation The color saturation.
 * @param pixelVolume The brightness of the pixels.
 * @param fadeAmount The speed at which the pixels fade.
 * @param delayDuration The duration between pixel brightness.
 */
class TwinklePixelsParams : public Parameters{
    public:
        /**
        * Consturctor parameters twinkle pixels animation.
        * Int Parameters: {hue, saturation, pixelVolume, fadeAmount, delayDuration}
        * Int parameters: {animPos}
        * @param hue The color hue.
        * @param saturation The color saturation.
        * @param pixelVolume The brightness of the pixels.
        * @param fadeAmount The speed at which the pixels fade.
        * @param delayDuration The duration between pixel brightness.
        */
       TwinklePixelsParams(int hue = 200, 
                           int saturation = 50, 
                           int pixelVolume = 50, 
                           int fadeAmount = 100, 
                           unsigned long  delayDuration = 50UL);
    
        ~ TwinklePixelsParams();
};

/**
 * Object containing parameters for the strobe animation.
 * Int Parameters: {red, greed, blue, timeOn, timeOff}
 * @param red The amount of red.
 * @param green The amount of green.
 * @param blue The amount of blue.
 * @param timeOn The amount of time spent on.
 * @param timeOff The amount of time spent off.
 */
class StrobeParams : public Parameters{
    public:
    //unsigned long strobeLastOn = 0;
    //unsigned long strobeLastOff = 0;
        /**
        * Consturctor parameters strobe animation.
        * Int Parameters: {red, greed, blue, timeOn, timeOff}
        * @param red The amount of red.
        * @param green The amount of green.
        * @param blue The amount of blue.
        * @param timeOn The amount of time spent on.
        * @param timeOff The amount of time spent off.
        */
       StrobeParams(int red = 255, 
                    int green = 255, 
                    int blue = 255,
                    unsigned long timeOn = 20UL, 
                    unsigned long timeOff = 55UL);
    
        ~ StrobeParams();
};

/**
 * Object containing parameters for the zip animation.
 * Int Parameters: {red, greed, blue, size, start, end}
 * Unsigned long Parameters: {delay}
 * @param red The amount of red.
 * @param green The amount of green.
 * @param blue The amount of blue.
 * @param size The the number of pixels of the zip.
 * @param start The position the animation start at.
 * @param end The position the animation ends at.
 * @param delay The speed of the animation.
 */
class ZipParams : public Parameters{ //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FIX: find out which is used for delay !!!!!!!!!!!!!!!!!!!!!!!!!
    public:
        /**
        * Consturctor parameters zip animation.
        * Int Parameters: {red, greed, blue, size, start, end}
        * Unsigned long Parameters: {delay}
        * @param red The amount of red.
        * @param green The amount of green.
        * @param blue The amount of blue.
        * @param size The the number of pixels of the zip.
        * @param start The position the animation start at.
        * @param end The position the animation ends at.
        * @param delay The speed of the animation.
        */
       ZipParams(int red = 0, 
                 int green = 0, 
                 int blue = 255,
                 int size = 2, 
                 int start = 10,
                 int end = NUM_LEDS,
                 unsigned long delay = 0UL);
    
        ~ ZipParams();
};

/**
 * Object containing parameters for the flash to beat animation.
 * Int Parameters: {red, green, blue}
 * Unsigned long parameters: {timeLeftOn} 
 * @param red The amount of red.
 * @param green The amount of green.
 * @param blue The amount of blue.
 * @param timeLeftOn The time the light is left on during a flash.
 */
class FlashToBeatParams : public Parameters{
    public:
    //bool flashToBeatIsOn = false; // Stops the flash from triggering if animation is already going !!!!!!! Bug is probably her !!!!!!!!
        /**
        * Consturctor parameters flash to beat animation.
        * Int Parameters: {red, greed, blue}
        * Unsigned long parameters: {timeLeftOn} 
        * @param red The amount of red.
        * @param green The amount of green.
        * @param blue The amount of blue.
        * @param timeLeftOn The time the light is left on during a flash.
        */
       FlashToBeatParams(int red = 255, 
                         int green = 255, 
                         int blue = 255,
                         unsigned long timeLeftOn = 50);
    
        ~ FlashToBeatParams();
};

class StackAnimParamRef{
    // ########################## Default function parameters ##################################
    private:
    static const bool defaultPrintSelect[nbParamType]; /* Default parameter for parameter selection in printParameters function */
    public:
    /* The code for the currently loaded aniamtion. */
    int animation;
    /* The last time the aniamtion was triggered */
    unsigned long timeLastActivate;
    /* The number of leds on this array. */
    int nbLeds;
    /* Value indicating werther the animation loaded into this stack object has position data. */
    bool hasPosData;
    // ########################### adjustable parameters ###############################
    int intAdjustParams[stackAdjustParamIntArraySize]; /* The array containing the adjustable integer parameters. */
    float floatAdjustParams[stackAdjustParamFloatArraySize]; /* The array containing the adjustable float parameters. */
    unsigned long uLongAdjustParams[stackAdjustParamLongArraySize]; /* The array containing the adjustable unsigned long parameters. */
    // ######################### non adjustable parameters #############################
    byte byteParams[stackParamByteArraySize]; /* The array containing the non adjustable byte parameters. */
    bool boolParams[stackParamBoolArraySize]; /* The array containing the non adjustable bool parameters. */
    int intParams[stackParamIntArraySize]; /* The array containing the non adjustable int parameters. !!! The first element of this list is the animation position !!!*/
    float floatParams[stackParamFloatArraySize]; /* The array containing the non adjustable float parameters. */
    unsigned long uLongParams[stackParamLongArraySize]; /* The array containing the non adjustable unsigned long parameters. */

    /**
     * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
     */
    void setAdjustParams(uint8_t paramIdx, int paramValue);

    /**
     * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
     */
    void setAdjustParams(uint8_t paramIdx, float paramValue);

    /**
     * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
     */
    void setAdjustParams(uint8_t paramIdx, unsigned long paramValue);

    /**
     * Prints the values for each of the selected parameters and for each of the selected animations.
     * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
     * @param aanimationSelect An array of booleans indicating which animation to print parameters for. (In the same order as animation code defined in Global constants)
     */
    void printParameters(const bool* printSelect  = defaultPrintSelect);


    StackAnimParamRef();
};

/**
 * Object storing all the animation parameters for a given array.
 */
class AnimParams{   
    // ########################## Default function parameters ################################
    private:
    static const bool defaultPrintSelect[nbParamType]; /* Default parameter for parameter selection in printParameters function */
    static const bool defaultAnimationSelect[NB_ANIMATIONS]; /* Default parameter for animation selection in printParameters function */
    // #######################################################################################
    public:
    const u_int16_t arrayId; /* The identifier number for the array this parameter object is attached to. */
    const int nbLeds; /* The number of LEDs the array the parameter object is attached to has. */
    Parameters* animationParameters[NB_ANIMATIONS]; /* The array of pointers to each animation parameter object. */

    /**
     * Sets the parameter value for the given parameter and animation.
     * @param animation The animation that will have its value set.
     * @param paramIdx The index of the parameter to be set.
     * @param paramValue The value the parameter is to set to.
     * @warning This function does not contain any out of bounds checks so as to increase performance.
    */
    void setAdjustParam(int animation, uint8_t paramIdx, int paramValue);

    /**
     * Sets the parameter value for the given parameter and animation.
     * @param animation The animation that will have its value set.
     * @param paramIdx The index of the parameter to be set.
     * @param paramValue The value the parameter is to set to.
     * @warning This function does not contain any out of bounds checks so as to increase performance.
     */
    void setAdjustParam(int animation, uint8_t paramIdx, float paramValue);

    /**
     * Sets the parameter value for the given parameter and animation.
     * @param animation The animation that will have its value set.
     * @param paramIdx The index of the parameter to be set.
     * @param paramValue The value the parameter is to set to.
     * @warning This function does not contain any out of bounds checks so as to increase performance.
     */
    void setAdjustParam(int animation, uint8_t paramIdx, unsigned long paramValue);

    /**
     * Finds the maximum number of parameters of each type. Used to set the current animation parameter values object stored staticaly on the stack.
     * @return Array containing the maximum number of parameters of each type amongst all the animations. 
     * {nbAdjustParamInt, nbAdjustParamFloat, nbAdjustParamLong, nbParamByte, nbParamBool, nbParamInt, nbParamFloat, nbParamLong}
     */
    uint8_t* getParamArraySizes();

    /**
     * Prints the maximum number of parameters of each type used in all animations. Utility function used to facilitate 
     * setting stack parameter array sizes.
     */
    void printMaxParamArraySize();

    /**
     * Prints the values for each of the selected parameters and for each of the selected animations.
     * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
     * @param aanimationSelect An array of booleans indicating which animation to print parameters for. (In the same order as animation code defined in Global constants)
     */
    void printParameters(const bool* printSelect  = defaultPrintSelect, const bool* animationSelect = defaultAnimationSelect);

    /**
     * Constructor  for object storing all the animation parameters for a given array.
     * @param nbLeds The number of LEDs the array the parameter object is attached to has.
     */
    AnimParams(int nbLeds = NUM_LEDS);

    ~AnimParams();
};

/**
 * Loads the animation parameter data from from the specified heap storage to the specified stack storage for
 * the animation specified by the stackParameters's animation variable.
 * 
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void copyValueFromHeap(AnimParams& heapParameters, StackAnimParamRef& stackParameters);

/**
 * Loads the animation parameter data from from the specified stack storage to the specified heap storage for
 * the animation specified by the stackParameters's animation variable.
 * 
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void copyValueToHeap(StackAnimParamRef& stackParameters, AnimParams& heapParameters);

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void setParam(StackAnimParamRef& stackParameters, AnimParams& heapParameters, uint8_t paramIdx, int paramValue);

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void setParam(StackAnimParamRef* &stackParameters, AnimParams& heapParameters, uint8_t paramIdx, float paramValue);

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void setParam(StackAnimParamRef& stackParameters, AnimParams& heapParameters, uint8_t paramIdx, unsigned long paramValue);

/**
 * Function that sets up all the stack stored values. Mostly from those stored in the heap.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void setupParameters(AnimParams* &heapParameters, StackAnimParamRef (&stackParameters)[NB_ARRAYS]);

#endif