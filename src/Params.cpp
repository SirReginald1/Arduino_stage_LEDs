#include "Arduino.h"
#include "Globals.h"
#include "Params.h"

const bool Parameters::defaultPrintSelect[nbParamType] = {true};

/**
 * Prints all the parameters.
 * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
 */
void Parameters::printParameters(const bool* printSelect){
    int i = 0;
    if(nbAdjustParamInt > 0 && printSelect[0]){
        Serial.print("AdjustParamInt: ");
        for(i=0;i<nbAdjustParamInt;i++){
            Serial.printf("%d, ", intAdjustParams[i]);
        }
        Serial.print("\n");
    }
    if(nbAdjustParamFloat > 0 && printSelect[1]){
        Serial.print("AdjustParamFloat: ");
        for(i=0;i<nbAdjustParamFloat;i++){
            Serial.printf("%d, ", floatAdjustParams[i]);
        }
        Serial.print("\n");
    }
    if(nbAdjustParamLong > 0 && printSelect[2]){
        Serial.print("AdjustParamLong: ");
        for(i=0;i<nbAdjustParamLong;i++){
            Serial.printf("%d, ", uLongAdjustParams[i]);
        }
        Serial.print("\n");
    }
    if(nbParamByte > 0 && printSelect[3]){
        Serial.print("ParamByte: ");
        for(i=0;i<nbParamByte;i++){
            Serial.printf("%d, ", byteParams[i]);
        }
        Serial.print("\n");
    }
    if(nbParamBool > 0 && printSelect[4]){
        Serial.print("ParamBool: ");
        for(i=0;i<nbParamBool;i++){
            Serial.printf("%d, ", boolParams[i]);
        }
        Serial.print("\n");
    }
    if(nbAdjustParamInt > 0 && printSelect[5]){
        Serial.print("AdjustParamInt: ");
        for(i=0;i<nbAdjustParamInt;i++){
            Serial.printf("%d, ", intParams[i]);
        }
        Serial.print("\n");
    }
    if(nbParamFloat > 0 && printSelect[6]){
        Serial.print("ParamFloat: ");
        for(i=0;i<nbParamFloat;i++){
            Serial.printf("%d, ", floatParams[i]);
        }
        Serial.print("\n");
    }
    if(nbParamLong > 0 && printSelect[7]){
        Serial.print("ParamLong: ");
        for(i=0;i<nbParamLong;i++){
            Serial.printf("%d, ", uLongParams[i]);
        }
        Serial.print("\n");
    }
}

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
Parameters::Parameters(uint8_t nbAdjustParamInt,
                       uint8_t nbAdjustParamFloat,
                       uint8_t nbAdjustParamLong,
                       uint8_t nbParamByte,
                       uint8_t nbParamBool,
                       uint8_t nbParamInt, 
                       uint8_t nbParamFloat, 
                       uint8_t nbParamLong):
    nbAdjustParamInt(nbAdjustParamInt),
    nbAdjustParamFloat(nbAdjustParamFloat),
    nbAdjustParamLong(nbAdjustParamLong),
    paramPosType(nullptr),
    nbParamByte(nbParamByte),
    nbParamBool(nbParamBool),
    nbParamFloat(nbParamFloat),
    nbParamInt(nbParamInt),
    nbParamLong(nbParamLong){
        if(nbAdjustParamInt != 0){
            intAdjustParams = new int[nbAdjustParamInt];
        }
        else{
            intAdjustParams = nullptr;
        }
        if(nbAdjustParamFloat != 0){
            floatAdjustParams = new float[nbAdjustParamFloat];
        }
        else{
            floatAdjustParams = nullptr;
        }
        if(nbAdjustParamLong != 0){
            uLongAdjustParams = new unsigned long[nbAdjustParamLong];
        }
        else{
            uLongAdjustParams = nullptr;
        }
        if(nbParamByte != 0){
            byteParams = new byte[nbParamByte];
        }
        else{
            byteParams = nullptr;
        }
        if(nbParamBool != 0){
            boolParams = new bool[nbParamBool];
        }
        else{
            boolParams = nullptr;
        }
        if(nbParamInt != 0){
            intParams = new int[nbParamInt];
        }
        else{
            intParams = nullptr;
        }
        if(nbParamFloat != 0){
            floatParams = new float[nbParamFloat];
        }
        else{
            floatParams = nullptr;
        }
        if(nbParamLong != 0){
            uLongParams = new unsigned long[nbParamLong];
        }
        else{
            uLongParams = nullptr;
        }
};

Parameters::Parameters(uint8_t paramTypeSizeList[nbParamType]):
        nbAdjustParamInt(paramTypeSizeList[0]),
        nbAdjustParamFloat(paramTypeSizeList[1]),
        nbAdjustParamLong(paramTypeSizeList[2]),
        paramPosType(nullptr),
        nbParamByte(paramTypeSizeList[3]),
        nbParamBool(paramTypeSizeList[4]),
        nbParamFloat(paramTypeSizeList[5]),
        nbParamInt(paramTypeSizeList[6]),
        nbParamLong(paramTypeSizeList[7]){
    if(nbAdjustParamInt != 0){
        intAdjustParams = new int[nbAdjustParamInt];
    }
    else{
        intAdjustParams = nullptr;
    }
    if(nbAdjustParamFloat != 0){
        floatAdjustParams = new float[nbAdjustParamFloat];
    }
    else{
        floatAdjustParams = nullptr;
    }
    if(nbAdjustParamLong != 0){
        uLongAdjustParams = new unsigned long[nbAdjustParamLong];
    }
    else{
        uLongAdjustParams = nullptr;
    }
    if(nbParamByte != 0){
        byteParams = new byte[nbParamByte];
    }
    else{
        byteParams = nullptr;
    }
    if(nbParamBool != 0){
        boolParams = new bool[nbParamBool];
    }
    else{
        boolParams = nullptr;
    }
    if(nbParamInt != 0){
        intParams = new int[nbParamInt];
    }
    else{
        intParams = nullptr;
    }
    if(nbParamFloat != 0){
        floatParams = new float[nbParamFloat];
    }
    else{
        floatParams = nullptr;
    }
    if(nbParamLong != 0){
        uLongParams = new unsigned long[nbParamLong];
    }
    else{
        uLongParams = nullptr;
    }
};

Parameters::~Parameters(){
    if(nbAdjustParamInt != 0){
        delete[] intAdjustParams;
    }
    if(nbAdjustParamFloat != 0){
        delete[] floatAdjustParams;
    }
    if(nbAdjustParamLong != 0){
        delete[] uLongAdjustParams;
    }
    if(nbParamByte != 0){
        delete[] byteParams;
    }
    if(nbParamBool != 0){
        delete[] boolParams;
    }
    if(nbParamInt != 0){
        delete[] intParams;
    }
    if(nbParamFloat != 0){
        delete[] floatParams;
    }
    if(nbParamLong != 0){
        delete[] uLongParams;
    }
}
// Maby add these methods later: https://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects

/**
* Constructor for Rainbow Cycle animation.
* Int Parameters: {int delay}
* Int parameters: {animPos}
* @param speed The speed of the animation.
*/
RainbowCycleParams::RainbowCycleParams(int speed) : Parameters(1, 0, 0, 0, 0, 1){
    hasPosData = true;
    intAdjustParams[0] = speed;
    intParams[0] = 0;
    paramPosType = new int[1]{PARAM_TYPE_CODE_INT};
}

RainbowCycleParams::~RainbowCycleParams(){}

/**
* Consturctor parameters for fade in and out animation.
* Int Parameters: {red, green, int blue, int speed}
* Int parameters: {animPos}
* @param red Amount of red.
* @param green Amount of green.
* @param blue Amount of blue.
* @param speed The speed of the animation.
*/
FadeInAndOutParams::FadeInAndOutParams(int red, 
                                       int green, 
                                       int blue, 
                                       unsigned long speed) : Parameters(4, 0, 1, 0, 1, 1){
    hasPosData = true;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    uLongAdjustParams[0] = (unsigned long) speed;
    boolParams[0] = true; // {FadingIn}
    intParams[0] = 0;
    paramPosType = new int[4]{PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_UNSIGNED_LONG};
 }
    
FadeInAndOutParams::~FadeInAndOutParams(){}

/**
* Consturctor parameters sparkle animation.
* Int Adjust Parameters: {red, green, blue, delay}
* Int parameters: {animPos}
* @param red Amount of red.
* @param green Amount of green.
* @param blue Amount of blue.
* @param delay The speed of the animation.
*/
SparkleParams::SparkleParams(int red, 
                             int green, 
                             int blue, 
                             int delay) : Parameters(4, 0, 0, 0, 0, 1){
    hasPosData = false;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    intAdjustParams[3] = delay;
    intParams[0] = 0; // The last pixel position
    paramPosType = new int[4]{PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT};
 }
    
SparkleParams::~SparkleParams(){}

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
FireParams::FireParams(int nbLeds, 
                       int flameHeight, 
                       int sparks, 
                       int delay, 
                       float fireIntensity) : Parameters(2, 1, 1, nbLeds){
    hasPosData = false;   
    intAdjustParams[0] = flameHeight;
    intAdjustParams[1] = sparks;
    uLongAdjustParams[0] = (unsigned long) delay;
    floatAdjustParams[0] = fireIntensity;
    for(int i=0;i<nbLeds;i++){
        byteParams[i] = (byte)0; // The fire heat array
    }
    paramPosType = new int[5]{PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_INT, PARAM_TYPE_CODE_FLOAT};
}
    
FireParams::~ FireParams(){
    //delete[] fireHeat;
}



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
ShootingStarParams::ShootingStarParams(int red, 
                                       int green, 
                                       int blue, 
                                       int tailLength, 
                                       unsigned long delayDuration, 
                                       unsigned long interval, 
                                       int direction) : Parameters(5, 0, 2, 0, 0, 1, 0, 1){
    hasPosData = true;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    intAdjustParams[3] = tailLength;
    //intAdjustParams[4] = delayDuration;
    //intAdjustParams[4] = interval;
    intAdjustParams[4] = direction;
    uLongAdjustParams[0] = delayDuration;
    uLongAdjustParams[1] = interval;
    intParams[0] = 0; // Animation position
    uLongParams[0] = 0U; // Last time a star aniamtion started
    paramPosType = new int[7]{PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT,
                              PARAM_TYPE_CODE_UNSIGNED_LONG, 
                              PARAM_TYPE_CODE_UNSIGNED_LONG, 
                              PARAM_TYPE_CODE_INT};
 }
    
ShootingStarParams::~ ShootingStarParams(){}


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
TwinklePixelsParams::TwinklePixelsParams(int hue, 
                                         int saturation, 
                                         int pixelVolume, 
                                         int fadeAmount, 
                                         unsigned long delayDuration) : Parameters(4, 0, 1, 0, 0, 1){
    hasPosData = false;
    intAdjustParams[0] = hue;
    intAdjustParams[1] = saturation;
    intAdjustParams[2] = pixelVolume;
    intAdjustParams[3] = fadeAmount;
    uLongAdjustParams[0] = delayDuration;
    intParams[0] = 0;
    paramPosType = new int[5]{PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_UNSIGNED_LONG};
}

TwinklePixelsParams::~TwinklePixelsParams(){}

/**
* Consturctor parameters strobe animation.
* Int Parameters: {red, greed, blue, timeOn, timeOff}
* @param red The amount of red.
* @param green The amount of green.
* @param blue The amount of blue.
* @param timeOn The amount of time spent on.
* @param timeOff The amount of time spent off.
*/
StrobeParams::StrobeParams(int red, 
                           int green, 
                           int blue,
                           unsigned long timeOn, 
                           unsigned long timeOff) : Parameters(3, 0, 2, 0, 0, 0, 0, 1){
    hasPosData = false;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    uLongAdjustParams[0] = timeOn;
    uLongAdjustParams[1] = timeOff;
    uLongParams[0] = 0; // Time last off. Time last on is timeLastActivate
    //uLongParams[1] = 0;
    paramPosType = new int[5]{PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_UNSIGNED_LONG, 
                              PARAM_TYPE_CODE_UNSIGNED_LONG};
}

StrobeParams::~StrobeParams(){}

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
ZipParams::ZipParams(int red, 
                     int green, 
                     int blue,
                     int size, 
                     int start,
                     int end,
                     unsigned long delay) : Parameters(6, 0, 1, 0, 0, 1){
    hasPosData = true;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    intAdjustParams[3] = size;
    intAdjustParams[4] = start;
    intAdjustParams[5] = end;
    uLongAdjustParams[0] = delay;
    intParams[0] = 0;
    paramPosType = new int[7]{PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT,  
                              PARAM_TYPE_CODE_UNSIGNED_LONG};
 }
    
ZipParams::~ZipParams(){}



/**
 * Consturctor parameters flash to beat animation.
 * Int Parameters: {red, greed, blue}
 * Unsigned long parameters: {timeLeftOn} 
 * @param red The amount of red.
 * @param green The amount of green.
 * @param blue The amount of blue.
 * @param timeLeftOn The time the light is left on during a flash.
*/
FlashToBeatParams::FlashToBeatParams(int red, 
                                     int green, 
                                     int blue,
                                     unsigned long timeLeftOn) : Parameters(3, 0, 1, 0, 1){
    hasPosData = false;
    intAdjustParams[0] = red;
    intAdjustParams[1] = green;
    intAdjustParams[2] = blue;
    uLongAdjustParams[0] = timeLeftOn;
    boolParams[0] = false; // Stops the flash from triggering if animation is already going !!!!!!! Bug is probably her !!!!!!!!
    paramPosType = new int[4]{PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT, 
                              PARAM_TYPE_CODE_INT,   
                              PARAM_TYPE_CODE_UNSIGNED_LONG};
}
    
FlashToBeatParams::~ FlashToBeatParams(){}


/**
 * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
 */
void StackAnimParamRef::setAdjustParams(uint8_t paramIdx, int paramValue){
    this->intAdjustParams[paramIdx] = paramValue;
}

/**
 * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
 */
void StackAnimParamRef::setAdjustParams(uint8_t paramIdx, float paramValue){
    this->floatAdjustParams[paramIdx] = paramValue;
}

/**
 * Used to set currently runing animation parameter values. Exists as separate function so that heap parameters can be set by other core.
 */
void StackAnimParamRef::setAdjustParams(uint8_t paramIdx, unsigned long paramValue){
    this->uLongParams[paramIdx] = paramValue;
}

static const bool defaultPrintSelect[nbParamType] = {true}; /* Default parameter for parameter selection in printParameters function */

/**
 * Prints the values for each of the selected parameters and for each of the selected animations.
 * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
 * @param aanimationSelect An array of booleans indicating which animation to print parameters for. (In the same order as animation code defined in Global constants)
 */
void printParameters(const bool* printSelect  = defaultPrintSelect){
    int intAdjustParams[stackAdjustParamIntArraySize]; /* The array containing the adjustable integer parameters. */
    float floatAdjustParams[stackAdjustParamFloatArraySize]; /* The array containing the adjustable float parameters. */
    unsigned long uLongAdjustParams[stackAdjustParamLongArraySize]; /* The array containing the adjustable unsigned long parameters. */
    // ######################### non adjustable parameters #############################
    byte byteParams[stackParamByteArraySize]; /* The array containing the non adjustable byte parameters. */
    bool boolParams[stackParamBoolArraySize]; /* The array containing the non adjustable bool parameters. */
    int intParams[stackParamIntArraySize]; /* The array containing the non adjustable int parameters. !!! The first element of this list is the animation position !!!*/
    float floatParams[stackParamFloatArraySize]; /* The array containing the non adjustable float parameters. */
    unsigned long uLongParams[stackParamLongArraySize]; /* The array containing the non adjustable unsigned long parameters. */
}


StackAnimParamRef::StackAnimParamRef(){
    animation = DEFAULT_ANIAMTION;
    timeLastActivate = 0UL;
    nbLeds = NUM_LEDS;
    hasPosData = false;
    int i = 0;
    for(i=0;i<stackAdjustParamIntArraySize;i++){
        intAdjustParams[i] = 0;
    }
    for(i=0;i<stackAdjustParamFloatArraySize;i++){
        floatAdjustParams[i] = 0.F;
    }
    for(i=0;i<stackAdjustParamLongArraySize;i++){
        uLongAdjustParams[i] = 0UL;
    }
    for(i=0;i<stackParamByteArraySize;i++){
        byteParams[i] = 0;
    }
    for(i=0;i<stackParamBoolArraySize;i++){
        boolParams[i] = false;
    }
    for(i=0;i<stackParamIntArraySize;i++){
        intParams[i] = 0;
    }
    for(i=0;i<stackParamFloatArraySize;i++){
        floatParams[i] = 0.F;
    }
    for(i=0;i<stackParamLongArraySize;i++){
        uLongParams[i] = 0UL;
    }
}


/**
 * Sets the parameter value for the given parameter and animation.
 * @param animation The animation that will have its value set.
 * @param paramIdx The index of the parameter to be set.
 * @param paramValue The value the parameter is to set to.
 * @warning This function does not contain any out of bounds checks so as to increase performance.
 */
void AnimParams::setAdjustParam(int animation, uint8_t paramIdx, int paramValue){
    this->animationParameters[animation]->intAdjustParams[paramIdx] = paramValue;
}

/**
 * Sets the parameter value for the given parameter and animation.
 * @param animation The animation that will have its value set.
 * @param paramIdx The index of the parameter to be set.
 * @param paramValue The value the parameter is to set to.
 * @warning This function does not contain any out of bounds checks so as to increase performance.
 */
void AnimParams::setAdjustParam(int animation, uint8_t paramIdx, float paramValue){
    this->animationParameters[animation]->floatAdjustParams[paramIdx] = paramValue;
}

/**
 * Sets the parameter value for the given parameter and animation.
 * @param animation The animation that will have its value set.
 * @param paramIdx The index of the parameter to be set.
 * @param paramValue The value the parameter is to set to.
 * @warning This function does not contain any out of bounds checks so as to increase performance.
 */
void AnimParams::setAdjustParam(int animation, uint8_t paramIdx, unsigned long paramValue){
    this->animationParameters[animation]->uLongAdjustParams[paramIdx] = paramValue;
}

/**
 * Finds the maximum number of parameters of each type. Used to set the current animation parameter values object stored staticaly on the stack.
 * @return Array containing the maximum number of parameters of each type amongst all the animations. 
 * {nbAdjustParamInt, nbAdjustParamFloat, nbAdjustParamLong, nbParamByte, nbParamBool, nbParamInt, nbParamFloat, nbParamLong}
 */
uint8_t* AnimParams::getParamArraySizes(){
    uint8_t* maxNbParams = new uint8_t[nbParamType];
    uint8_t currentVal = 0; // Using variable is faster as it's stored in stack
    for(int i=0;i<NB_ANIMATIONS;i++){
        currentVal = animationParameters[i] -> nbAdjustParamInt;
        if(currentVal > maxNbParams[0]){
            maxNbParams[0] = currentVal; 
        }
        currentVal = animationParameters[i] -> nbAdjustParamFloat;
        if(currentVal > maxNbParams[1]){
            maxNbParams[1] = currentVal; 
        }
        currentVal = animationParameters[i] -> nbAdjustParamLong; 
        if(currentVal > maxNbParams[2]){
            maxNbParams[2] = currentVal;
        }
        currentVal = animationParameters[i] -> nbParamByte;
        if(currentVal > maxNbParams[3]){
            maxNbParams[3] = currentVal;
        }
        currentVal = animationParameters[i] -> nbParamBool;
        if(currentVal > maxNbParams[4]){
            maxNbParams[4] = currentVal;
        }
        currentVal = animationParameters[i] -> nbParamInt;
        if(currentVal > maxNbParams[5]){
            maxNbParams[5] = currentVal;
        }
        currentVal = animationParameters[i] -> nbParamFloat;
        if(currentVal > maxNbParams[6]){
            maxNbParams[6] = currentVal;
        }
        currentVal = animationParameters[i] -> nbParamLong;
        if(currentVal > maxNbParams[7]){
            maxNbParams[7] = currentVal;
        }
    }
    return maxNbParams;
}

/**
 * Prints the maximum number of parameters of each type used in all animations. Utility function used to facilitate 
 * setting stack parameter array sizes.
 */
void AnimParams::printMaxParamArraySize(){
    uint8_t* maxArraySizes = getParamArraySizes();
    Serial.printf("Max Adjust int param: %d\n", maxArraySizes[0]);
    Serial.printf("Max Adjust float param: %d\n", maxArraySizes[1]);
    Serial.printf("Max Adjust u long param: %d\n", maxArraySizes[2]);
    Serial.printf("Max byte param: %d\n", maxArraySizes[3]);
    Serial.printf("Max bool param: %d\n", maxArraySizes[4]);
    Serial.printf("Max int param: %d\n", maxArraySizes[5]);
    Serial.printf("Max float param: %d\n", maxArraySizes[6]);
    Serial.printf("Max u long param: %d\n", maxArraySizes[7]);
}
    
/**
 * Default constructor for object storing all the animation parameters for a given array.
 */
//AnimParams(): arrayId(nbArraysInitialized), nbLeds(NUM_LEDS){
//    nbArraysInitialized++;
//    animationParameters[0] = new RainbowCycleParams();
//    animationParameters[1] = new RainbowCycleParams();
//    //animationParameters[1] = new FadeInAndOutParams();
//    //animationParameters[2] = new SparkleParams();
//    //animationParameters[3] = new FireParams(nbLeds);
//    //animationParameters[4] = new ShootingStarParams();
//    //animationParameters[5] = new TwinklePixelsParams();
//    //animationParameters[6] = new StrobeParams();
//    //animationParameters[7] = new ZipParams();
//    //animationParameters[8] = new FlashToBeatParams();
//}

const bool AnimParams::defaultPrintSelect[nbParamType] = {true};
const bool AnimParams::defaultAnimationSelect[NB_ANIMATIONS] = {true};

/**
 * Prints the values for each of the selected parameters and for each of the selected animations.
 * @param printSelect An array of booleans indicating which parameters to print. (Same order as in constructor parameters)
 * @param aanimationSelect An array of booleans indicating which animation to print parameters for. (In the same order as animation code defined in Global constants)
 */
void AnimParams::printParameters(const bool* printSelect, const bool* animationSelect){
    Serial.println("Parameter values:");
    for(int i=0;i<NB_ANIMATIONS;i++){
        if(defaultAnimationSelect){
            Serial.printf("Animation %d: ", i);
            animationParameters[i]->printParameters(defaultPrintSelect);
        }
    }
}

/**
 * Constructor  for object storing all the animation parameters for a given array.
 * @param nbLeds The number of LEDs the array the parameter object is attached to has.
 */
AnimParams::AnimParams(int nbLeds): arrayId(nbArraysInitialized), nbLeds(nbLeds){
    nbArraysInitialized++;
    animationParameters[0] = new RainbowCycleParams();
    animationParameters[1] = new FadeInAndOutParams();
    animationParameters[2] = new SparkleParams();
    animationParameters[3] = new FireParams(nbLeds);
    animationParameters[4] = new ShootingStarParams();
    animationParameters[5] = new TwinklePixelsParams();
    animationParameters[6] = new StrobeParams();
    animationParameters[7] = new ZipParams();
    animationParameters[8] = new FlashToBeatParams();
}

AnimParams::~AnimParams(){
    //delete[] animationParameters;
}


/**
 * Loads the animation parameter data from from the specified heap storage to the specified stack storage for
 * the animation specified by the stackParameters's animation variable.
 * 
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void copyValueFromHeap(AnimParams& heapParameters, StackAnimParamRef& stackParameters){
    //Parameters* tempParameters = heapParameters.animationParameters[stackParameters.animation];
    //int i = 0;
    //for(i=0;i<tempParameters->nbAdjustParamInt;i++){
    //    stackParameters.intAdjustParams[i] = tempParameters->intAdjustParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbAdjustParamFloat;i++){
    //    stackParameters.floatAdjustParams[i] = tempParameters->floatAdjustParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbAdjustParamLong;i++){
    //    stackParameters.uLongAdjustParams[i] = tempParameters->uLongAdjustParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbParamByte;i++){
    //    stackParameters.byteParams[i] = tempParameters->byteParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbParamBool;i++){
    //    stackParameters.boolParams[i] = tempParameters->boolParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbParamInt;i++){
    //    stackParameters.intParams[i] = tempParameters->intParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbParamFloat;i++){
    //    stackParameters.floatParams[i] = tempParameters->floatParams[i];
    //
    //}
    //for(i=0;i<tempParameters->nbParamLong;i++){
    //    stackParameters.uLongParams[i] = tempParameters->uLongParams[i];
    //
    //}
    Parameters* tempParameters = heapParameters.animationParameters[stackParameters.animation];
    stackParameters.timeLastActivate = tempParameters->timeLastActivate;
    stackParameters.hasPosData = tempParameters->hasPosData;
    if (tempParameters->nbAdjustParamInt > 0) {
        memcpy(stackParameters.intAdjustParams, tempParameters->intAdjustParams, tempParameters->nbAdjustParamInt * sizeof(int));
    }
    if (tempParameters->nbAdjustParamFloat > 0) {
        memcpy(stackParameters.floatAdjustParams, tempParameters->floatAdjustParams, tempParameters->nbAdjustParamFloat * sizeof(float));
    }
    if (tempParameters->nbAdjustParamLong > 0) {
        memcpy(stackParameters.uLongAdjustParams, tempParameters->uLongAdjustParams, tempParameters->nbAdjustParamLong * sizeof(unsigned long));
    }
    if (tempParameters->nbParamByte > 0) {
        memcpy(stackParameters.byteParams, tempParameters->byteParams, tempParameters->nbParamByte * sizeof(byte));
    }
    if (tempParameters->nbParamBool > 0) {
        memcpy(stackParameters.boolParams, tempParameters->boolParams, tempParameters->nbParamBool * sizeof(bool));
    }
    if (tempParameters->nbParamInt > 0) {
        memcpy(stackParameters.intParams, tempParameters->intParams, tempParameters->nbParamInt * sizeof(int));
    }
    if (tempParameters->nbParamFloat > 0) {
        memcpy(stackParameters.floatParams, tempParameters->floatParams, tempParameters->nbParamFloat * sizeof(float));
    }
    if (tempParameters->nbParamLong > 0) {
        memcpy(stackParameters.uLongParams, tempParameters->uLongParams, tempParameters->nbParamLong * sizeof(unsigned long));
    }
}

/**
 * Loads the animation parameter data from from the specified stack storage to the specified heap storage for
 * the animation specified by the stackParameters's animation variable.
 * 
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void copyValueToHeap(StackAnimParamRef& stackParameters, AnimParams& heapParameters) {
    Parameters* tempParameters = heapParameters.animationParameters[stackParameters.animation];
    tempParameters->timeLastActivate = stackParameters.timeLastActivate;
    if (tempParameters->nbAdjustParamInt > 0) {
        memcpy(tempParameters->intAdjustParams, stackParameters.intAdjustParams, sizeof(tempParameters->intAdjustParams)); //tempParameters->nbAdjustParamInt * sizeof(int));
    }
    if (tempParameters->nbAdjustParamFloat > 0) {
        memcpy(tempParameters->floatAdjustParams, stackParameters.floatAdjustParams, sizeof(tempParameters->floatAdjustParams));// tempParameters->nbAdjustParamFloat * sizeof(float));
    }
    if (tempParameters->nbAdjustParamLong > 0) {
        memcpy(tempParameters->uLongAdjustParams, stackParameters.uLongAdjustParams, sizeof(tempParameters->uLongAdjustParams)); //tempParameters->nbAdjustParamLong * sizeof(unsigned long));
    }
    if (tempParameters->nbParamByte > 0) {
        memcpy(tempParameters->byteParams, stackParameters.byteParams, sizeof(tempParameters->byteParams)); //tempParameters->nbParamByte * sizeof(byte));
    }
    if (tempParameters->nbParamBool > 0) {
        memcpy(tempParameters->boolParams, stackParameters.boolParams, sizeof(tempParameters->boolParams)); //tempParameters->nbParamBool * sizeof(bool));
    }
    if (tempParameters->nbParamInt > 0) {
        memcpy(tempParameters->intParams, stackParameters.intParams, sizeof(tempParameters->intAdjustParams)); //tempParameters->nbParamInt * sizeof(int));
    }
    if (tempParameters->nbParamFloat > 0) {
        memcpy(tempParameters->floatParams, stackParameters.floatParams, sizeof(tempParameters->floatParams)); //tempParameters->nbParamFloat * sizeof(int));
    }
    if (tempParameters->nbParamLong > 0) {
        memcpy(tempParameters->uLongParams, stackParameters.uLongParams, sizeof(tempParameters->uLongParams)); //tempParameters->nbParamLong * sizeof(unsigned long));
    }
}

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void switchAnimationsNonOpti(StackAnimParamRef& stackParameters, AnimParams& heapParameters, uint8_t paramIdx, int paramValue){
    stackParameters.setAdjustParams(paramIdx, paramValue);
    heapParameters.setAdjustParam(stackParameters.animation, paramIdx, paramValue);
}

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void switchAnimationsNonOpti(StackAnimParamRef* &stackParameters, AnimParams& heapParameters, uint8_t paramIdx, float paramValue){
    stackParameters -> setAdjustParams(paramIdx, paramValue);
    heapParameters.setAdjustParam(stackParameters -> animation, paramIdx, paramValue);
}

/**
 * Function used to switch between animations. Will set both stack and heap animation variables at once.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 * @param paramIdx The index of the parameter being set. Used to place parameters in the appropriate parameter array.
 * @param paramValue The value the parameter is to be set to.
 */
void switchAnimationsNonOpti(StackAnimParamRef& stackParameters, AnimParams& heapParameters, uint8_t paramIdx, unsigned long paramValue){
    stackParameters.setAdjustParams(paramIdx, paramValue);
    heapParameters.setAdjustParam(stackParameters.animation, paramIdx, paramValue);
}

/**
 * Function that sets up all the stack stored values. Mostly from those stored in the heap.
 * @param heapParameters Reference to heap parameter storage.
 * @param stackParameters Reference to stack parameter storage.
 */
void setupParameters(AnimParams* &heapParameters, StackAnimParamRef (&stackParameters)[NB_ARRAYS]){
    for(int i=0;i<NB_ARRAYS;i++){
        copyValueFromHeap(heapParameters[i], stackParameters[i]);
        stackParameters[i].nbLeds = heapParameters[i].nbLeds; 
    }
}