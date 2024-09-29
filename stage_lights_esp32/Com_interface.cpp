/**
* NOTES:
* All comands passed to the controller must be surounded by START_MARKER and END_MARKER to be parsed.
* In order to switch between data parsing modes use MODE_CHANGE_FLAG as first character followed by the desiered data parsing mode.
*
* At present the data parsing mode is automaticaly set to RECIVE_MODE_ANIM_SELECT at each animation change. Except for animation 7 where it switches to RECIVE_MODE_RUN_PREP_ANIM.
* If stuck on animation 7 send "@0<0,1>".
*/ 

#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <FastLED.h>
#include "Globals.h"
#include "Animations.h"
#include "SD_manager.h"

// ################### Compilation options ####################
#define START_MARKER '<'
#define END_MARKER '>'
/** Receving this symbole signals the controler to start somthing.*/
#define START_ACTION_MARKER '#'
/** Receving this symbole signals the controler to stop somthing.*/
#define STOP_ACTION_MARKER '!'
// #################### Comunication reception mode #####################
/** The flag used to signal that the following integer is to be interpreted as mode comunication mode change.*/
#define MODE_CHANGE_FLAG '@'
/** The mode number for the animation and parameter selection mode.*/
#define RECIVE_MODE_ANIM_SELECT 0
/** The mode number for switching on the microphone and beat detection.*/
#define RECIVE_MODE_SWITCH_ON_BEAT_DETECT 1
/** The mode number for the preprepared animation mode.*/
#define RECIVE_MODE_SET_BRIGHTNESS 2
/** The mode number for the frequency band setting mode.*/
#define RECIVE_MODE_SET_FREQ_BAND 4
/** The mode number for the preprepared animation mode.*/
#define RECIVE_MODE_RUN_PREP_ANIM 5

/** The task handler for the FFT task runing on core 0 */
extern TaskHandle_t mainCore0Handle;
/** The value indicating if the mic and FFT loop is runing on core0. */
extern bool appModeMicFFTOnCore1;

/** The notification queue used by core0 to signal core 1 */
extern QueueHandle_t core0FreqBandQueue;

/** The LED arrays */
extern CRGB led_arrays[NB_ARRAYS][NUM_LEDS];

class ComInterface{
  public:
    // ################## Comunication variables ##################
    // The value that indicates which animation to run.
    //static int animation;
    // The index of the array currently being manipulated
    static int currentArray;
    // The number of interger parameters that are being used by each array
    static int paramCountInt[NB_ARRAYS];
    // The number of float parameters that are being used by each array
    static float paramCountFloat[NB_ARRAYS];
    // The number of unsigned long parameters that are being used by each array
    static unsigned long paramCountUnsignedLong[NB_ARRAYS];

    /**Value indicating */
    static bool runingAction;

    // ################### Reading data code ##################
    /*The maximum number of bytes that the buffer can recive.*/
    static const byte numChars = 32;

    /*The buffer in which receved strings stored. Has a terminating character (\0).*/
    static char receivedChars[numChars];

    /*Temporary array for use when parsing*/
    static char tempChars[numChars];        

    /**
    -Boolean indicating if there is data being proccesed.
    If true will not read and parse from the buffer. Is set back to false once all data reved data hase been delt with.
    */
    static boolean newData;

    /**The mode used to determin how receved data is parsed. In sync with the curently used functionality.*/
    static int dataParsingMode;

    // ###################### Getter an setter functions ######################################
    /*
    static void setAnimation(int value);
    static int getAnimation();
    */
    static void setCurrentArray(int value);
    static int getCurrentArray();

    static void setParamCountInt(int value);
    static int getParamCountInt();


    static void initParametersArray();

    /**
    * Function that takes the animation number and the number of the current parameter for that animation function and returns the code for the type of that variable.
    * !!!!!!! Any parameter that is not directly related to the changing how the annimation runs such as current millis value or total number of LEDs in array is not 
    * is skiped when counting the parameter number !!!!!!!
    * 
    * @param animation_in The animation code for the current animation.
    *
    * @param param_buffer_count_in The position of the current parameter in the specified animation function. Non animation related parameters are not counted in the count. 
    *
    * @return The code for the type of the current parameter. {0: int, 1: float, 2: unsigned long}
    */
    static int findParamType(int animation_in, int param_buffer_count_in);

    static void serialFlush();

    static int separate(String str, char **p, int size);

    /**
      * Takes data from the buffer
    */
    static void recvWithStartEndMarkers();

    //============
    /*This function parses the data that hase been receved*/
    static void parseAnimationChangeData();
    /**This function deals with parsing data in a default general context.*/
    static void parsePreprepAnimData();

    static void parseBrightnessData();

    static void parseSetFreqBandData();

    static void swithMicFFTMode();

    static void readInput();
};

// #########################################################################################################################
// ############################################# STATIC VARIABLE DEFINITION ################################################
// #########################################################################################################################

boolean ComInterface::newData = false;

//int ComInterface::animation = 1;

int ComInterface::currentArray = 0;

//animParamRef ComInterface::animParamRefs[NB_ARRAYS];

// The number of interger parameters that are being used by each array
int ComInterface::paramCountInt[NB_ARRAYS] = {0};
// The number of float parameters that are being used by each array
float ComInterface::paramCountFloat[NB_ARRAYS] = {0};
// The number of unsigned long parameters that are being used by each array
unsigned long ComInterface::paramCountUnsignedLong[NB_ARRAYS] = {0};

/*The buffer in which receved strings stored. Has a terminating character (\0).*/
char ComInterface::receivedChars[numChars] = {0};

/*Temporary array for use when parsing*/
char ComInterface::tempChars[numChars] = {0};

/**VFalue indicating if the controller has been ordered to start runing an action.*/
bool ComInterface::runingAction = false;

/**The mode used to determin how receved data is parsed. In sync with the curently used functionality.*/
int ComInterface::dataParsingMode = 0;

// #########################################################################################################################
// ######################################### GETTER AND SETTER FUNCTION DEFINITION #########################################
// #########################################################################################################################
/*
void ComInterface::setAnimation(int value){
  animation = value;
}

int ComInterface::getAnimation(){
  return animation;
}
*/
void ComInterface::setCurrentArray(int value){
  currentArray = value;
}

int ComInterface::getCurrentArray(){
  return currentArray;
}

// #########################################################################################################################
// ############################################# STATIC FUNCTION DEFINITION ################################################
// #########################################################################################################################
/**
  * Function that takes the animation number and the number of the current parameter for that animation function and returns the code for the type of that variable.
  * !!!!!!! Any parameter that is not directly related to the changing how the annimation runs such as current millis value or total number of LEDs in array is not 
  * is skiped when counting the parameter number !!!!!!!
  * 
  * @param animation_in The animation code for the current animation.
  *
  * @param param_buffer_count_in The position of the current parameter in the specified animation function. Non animation related parameters are not counted in the count. 
  *
  * @return The code for the type of the current parameter. {0: int, 1: float, 2: unsigned long}
*/
int ComInterface::findParamType(int animation_in, int param_buffer_count_in){
  // TODO: Add functions in animation.cpp for each animation that return their type so that all animation modifiaction do not necesitate modifications in this file.
  // TODO: Add a dictionary referencing annimation codes and type indicator codes so as to make them flexible.

  /*
  * Parameter type value indicator.
  * 0: int.
  * 1: float.
  * 2: unsigned long.
  */
  int type_indicator_out;
  // This switch identifies the type of the parameter in question.
    switch (animation_in) {
      // ################# The Sparkle animation #######################
      /*
      case 3: 
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) { // Starts at 0
          case 3:
            type_indicator_out = 0; // !!!!!!!!!!!!!!!!!!!!!!!! Change this to = 2 when you change the delay functioning !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            break;
          default:
            type_indicator_out = 0;
        }
        break;
        */
      // ###########################################################
      // ################# The Fire animation #######################
      case ANIM_CODE_FIRE:
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) {
          case 3:
            type_indicator_out = 1;
            break;
          default:
            type_indicator_out = 0;
        }
        break;
      // ###########################################################
      // ################# The zip animation #######################
      case ANIM_CODE_ZIP:
        // This switch looks at which parameter in the function the current parameter is.
        switch (param_buffer_count_in) {
          case 4:
            type_indicator_out = 2;
            break;
          default:
            type_indicator_out = 0;
        }
        break;
      // ###########################################################
      default:
        type_indicator_out = 0;
    }
  return type_indicator_out;
}

void ComInterface::serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

int ComInterface::separate(String str, char **p, int size){
  int n;
  char s[100];
  strcpy(s, str.c_str());
  *p++ = strtok (s, ",");
  for (n = 1; NULL != (*p++ = strtok(NULL, ",")); n++)
      if (size == n)
          break;
  return n;
}

/*This function is requiered for boards like the arduino that have trouble receving data over serial*/
void ComInterface::recvWithStartEndMarkers() {
  static byte ndx = 0;
  char rc;
  rc = Serial.read();
  // Checks the first character which is the flag that indicates which parsing mode will be used
  // The second character is the parsing mode to be used
  switch(rc){
    case START_MARKER:
      break;
    case MODE_CHANGE_FLAG:
      try{
        ComInterface::dataParsingMode = Serial.read()-48; //Serial.parseInt();
        Serial.read();
      }
      catch(...){
        // Do nothing
      }
      break;
    default:
      break;
  }
  while (Serial.available() > 0 && newData == false) {
      rc = Serial.read();
      if (rc != END_MARKER) {
          receivedChars[ndx] = rc;
          ndx++;
          if (ndx >= numChars) {
              ndx = numChars - 1;
          }
      }
      else {
          receivedChars[ndx] = '\0'; // terminate the string
          //recvInProgress = false;
          ndx = 0;
          newData = true;
      }
  }
}

/*This function parses the data that hase been receved*/
void ComInterface::parseAnimationChangeData() {      // split the data into its parts
  extern animParamRef animParamRefs[NB_ARRAYS];
  /*Value extracted from buffer*/
  char * strtokIndx;
  /*Counts the number of parameters after the anomation value in current data parsing.*/
  int param_buffer_count = 0;
  /*
  * Parameter type value indicator.
  * 0: int
  * 1: float
  * 2: unsigned long
  */
  int type_indicator = 0;
  /*
  * The variable that holds the value of the parameter once casted to an int.
  */
  int parameter_value_int;
  /*
  * The variable that holds the value of the parameter once casted to an float.
  */
  float parameter_value_float;
  /*
  * The variable that holds the value of the parameter once casted to an unsigned long.
  */
  unsigned long parameter_value_unsigned_long;

  int animation = 0;

  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  currentArray = atoi(strtokIndx); // The current array being manipulated

  // Check that currentArray is acceptable
  if(currentArray < -2 || currentArray >= NB_ARRAYS){
    Serial.println("Invalide array!");
    currentArray = 0;
    return;
  }

  // The second element in the list
  strtokIndx = strtok(NULL, ",");

  animation = atoi(strtokIndx); // The animation to be switched to

  // Check that entered annimation value is valid
  if(animation < 0 || animation >= NB_ANIMATIONS){
    //ComInterface::animation = 1;
    Serial.println("Invalid animation!");
    return;
  }

  strtokIndx = strtok(NULL, ",");

  // Checks to see if array is == -1 which means change parameters for all arrays. -2 means change parameters for all and switch all arrays to animation
  if(currentArray == -1 || currentArray == -2){
    
    // If currentArray == -2 also switch all animations to specified animation
    if(currentArray == -2){
      for(int i=0;i<NB_ARRAYS;i++){
        animParamRefs[i].animation = animation;
      }
    }

    // For each subsequent parameter define apropriate type and sets its value in the the appropriate reference table.
    while(strtokIndx != NULL) {
      // Finds the type of the current parameter value
      type_indicator = findParamType(animation, param_buffer_count);
      // This switch casts the parameter to the appropriate type and sets the appropriate possition in the appropriat table with that value.
      switch (type_indicator) {
        case 1:
          parameter_value_float = atof(strtokIndx);
          for(int i=0;i<NB_ARRAYS;i++){
            Animations::setParametersFloat(animParamRefs, i, animation, param_buffer_count, parameter_value_float);
          }
          break;
        case 2:
          parameter_value_unsigned_long  = (unsigned long)atoi(strtokIndx);
          for(int i=0;i<NB_ARRAYS;i++){
            Animations::setParametersUnsignedLong(animParamRefs, i, animation, param_buffer_count, parameter_value_unsigned_long);
          }
          break;
        default:
          parameter_value_int  = atoi(strtokIndx);
          for(int i=0;i<NB_ARRAYS;i++){
            Animations::setParametersInt(animParamRefs, i, animation, param_buffer_count, parameter_value_int);
          }
      }
      // For each iteration of the loop will tell 
      param_buffer_count++;
      // Leave this at end
      strtokIndx = strtok(NULL, ",");
    }
  }
  else{

    animParamRefs[currentArray].animation = animation;

    // For each subsequent parameter define apropriate type and sets its value in the the appropriate reference table.
    while(strtokIndx != NULL) {
      // Finds the type of the current parameter value
      type_indicator = findParamType(animation, param_buffer_count);
      // This switch casts the parameter to the appropriate type and sets the appropriate possition in the appropriat table with that value.
      switch (type_indicator) {
        case 1:
          parameter_value_float = atof(strtokIndx);
          Animations::setParametersFloat(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_float);
          break;
        case 2:
          parameter_value_unsigned_long  = (unsigned long)atoi(strtokIndx);
          Animations::setParametersUnsignedLong(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_unsigned_long);
          break;
        default:
          parameter_value_int  = atoi(strtokIndx);
          Animations::setParametersInt(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_int);
      }
      // For each iteration of the loop will tell 
      param_buffer_count++;
      // Leave this at end
      strtokIndx = strtok(NULL, ",");
    }
  }
}

/**
  * This function deals with parsing data when setting the brightness.
*/
void ComInterface::parseBrightnessData(){
  //Serial.println("Enter set brightness data parsing."); ///////////////////////////////////////////////////////////////////////////////////
  /*Value extracted from buffer*/
  char * strtokIndx;
  /* The current array being manipulated */
  int currentArray;
  /* The brightness value */
  uint8_t brightness;

  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  currentArray = atoi(strtokIndx); // The current array being manipulated
  //Serial.print("Current array: ");Serial.println(currentArray); ///////////////////////////////////////////////////////////////////////////////////

  // Check that currentArray is acceptable
  if((currentArray < 0 && currentArray != -2) || currentArray >= NB_ARRAYS){
    //Serial.println("Invalide array!");
    currentArray = 0;
    return;
  }

  // The second element in the list
  strtokIndx = strtok(NULL, ",");
  brightness = (uint8_t)atoi(strtokIndx); // The brightness value
  //Serial.print("brightness: ");Serial.println(brightness); ///////////////////////////////////////////////////////////////////////////////////
  if(currentArray == -2){
    //Serial.print("Setting global brightness to: "); Serial.println(brightness);
    FastLED.setBrightness(brightness);
    //FastLED.show();  // Update the strip
  }
  else{
    for(int i=0;i<NUM_LEDS;i++){
      led_arrays[currentArray][i].r  = scale8(led_arrays[currentArray][i].r, brightness);
      led_arrays[currentArray][i].g  = scale8(led_arrays[currentArray][i].g, brightness);
      led_arrays[currentArray][i].b  = scale8(led_arrays[currentArray][i].b, brightness);
    }
  }
  
  // The second element in the list
  //strtokIndx = strtok(NULL, ",");
  //while(strtokIndx != NULL) {
  //  break;
  //}
  // Leave this at end
  strtokIndx = strtok(NULL, ",");
}

/**
  * This function deals with parsing data when setting frequency bands.
*/
void ComInterface::parseSetFreqBandData(){
  /*Value extracted from buffer*/
  char * strtokIndx;

  float value;

  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  

  while(strtokIndx != NULL) {
    value = atof(strtokIndx);
    xQueueSend(core0FreqBandQueue, &value, 5);
    strtokIndx = strtok(NULL, ",");
  }
  xTaskNotify(mainCore0Handle, MODE_SET_FREQ_BAND, eSetValueWithOverwrite);
  // Leave this at end
  strtokIndx = strtok(NULL, ",");
}

/**
  * This function deals with parsing data when preprepared animation mode is active.
*/
void ComInterface::parsePreprepAnimData(){
  /*Value extracted from buffer*/
  char * strtokIndx;
  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  
  // Switch that deals with potential flags in fist position.
  switch((int)strtokIndx){
    case (int)START_ACTION_MARKER:
      ComInterface::runingAction = true;
      Animations::flashToBeatGo = true;
      break;
    case (int)STOP_ACTION_MARKER:
      ComInterface::runingAction = false;
      Animations::stopFlashToBeatArray();
      break;
  }
  // The second element in the list
  strtokIndx = strtok(NULL, ",");
  while(strtokIndx != NULL) {
    break;
  }
  // Leave this at end
  strtokIndx = strtok(NULL, ",");
}

/**
  * Function used to parse data when MODE_MIC_FFT_ON is receved.
*/
void ComInterface::swithMicFFTMode(){
  //if(mainCore0Handle == NULL){
    uint32_t notifValue;
    /*Value extracted from buffer*/
    char * strtokIndx;
    // The first element in the list
    strtokIndx = strtok(tempChars,",");
    switch (atoi(strtokIndx)){
      case 1:
        notifValue = MODE_ADC_FFT_ON;
        break;
      case 2:
        notifValue = MODE_MIC_FFT_ON;
        break;
      case 0:
        notifValue = MODE_FFT_OFF;
        break;
      default:
        Serial.println("Parameter value not recognized!");
        break;
    }
    xTaskNotify(mainCore0Handle, notifValue, eSetValueWithOverwrite); //MODE_MIC_FFT_ON
    appModeMicFFTOnCore1 = !appModeMicFFTOnCore1;
  //}
}

/**
  * Function called in main program to wich will read and parse the data present in the input buffer depending on the mode it is given as parameter.
*/
void ComInterface::readInput(){ 
  ComInterface::recvWithStartEndMarkers();
  if (newData == true) {
      strcpy(tempChars, receivedChars);
          // this temporary copy is necessary to protect the original data
          //   because strtok() used in parseData() replaces the commas with \0
      switch(ComInterface::dataParsingMode){
        case RECIVE_MODE_ANIM_SELECT:
          ComInterface::parseAnimationChangeData();
          break;
        case RECIVE_MODE_SWITCH_ON_BEAT_DETECT:
            //ComInterface::parseAnimationChangeData();
            ComInterface::swithMicFFTMode(); // Doesn't switch data parsing modes as Mic animations will use the same as other.
            ComInterface::dataParsingMode = RECIVE_MODE_ANIM_SELECT;
          break;
        case RECIVE_MODE_SET_BRIGHTNESS:
          ComInterface::parseBrightnessData();
          ComInterface::dataParsingMode = RECIVE_MODE_ANIM_SELECT;
          break;
        case RECIVE_MODE_SET_FREQ_BAND:
          ComInterface::parseSetFreqBandData();
          ComInterface::dataParsingMode = RECIVE_MODE_ANIM_SELECT;
          break;
        case RECIVE_MODE_RUN_PREP_ANIM:
          ComInterface::parsePreprepAnimData();
          ComInterface::dataParsingMode = RECIVE_MODE_ANIM_SELECT;
          break;
      }
      newData = false;
  }

  // This ensures that once switched to animation mode 10 all subsequent messages are read with other mode untille specified otherwise.
  /*
  if(ComInterface::animation == 10){
    Serial.println("Switching parsing modes.");
    ComInterface::dataParsingMode = RECIVE_MODE_RUN_PREP_ANIM;
  }
  else{
    ComInterface::dataParsingMode = RECIVE_MODE_ANIM_SELECT;
  }
  */

}