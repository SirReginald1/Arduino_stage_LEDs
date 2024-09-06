#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Globals.h"
#include "Animations.h"

// ################### Compilation options ####################
#define START_MARKER '<'
#define END_MARKER '>'


class ComInterface{
  public:
    // ################## Comunication variables ##################
    // The value that indicates which animation to run.
    static int animation;
    // The index of the array currently being manipulated
    static int currentArray;
    // The number of interger parameters that are being used by each array
    static int paramCountInt[NB_ARRAYS];
    // The number of float parameters that are being used by each array
    static float paramCountFloat[NB_ARRAYS];
    // The number of unsigned long parameters that are being used by each array
    static unsigned long paramCountUnsignedLong[NB_ARRAYS];

    /** The array of structs that contain*/
    //static animParamRef animParamRefs[NB_ARRAYS];

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

    // ###################### Getter an setter functions ######################################

    static void setAnimation(int value);
    static int getAnimation();

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

    static void readInput();
};

// #########################################################################################################################
// ############################################# STATIC VARIABLE DEFINITION ################################################
// #########################################################################################################################

boolean ComInterface::newData = false;

int ComInterface::animation = 1;

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

// #########################################################################################################################
// ######################################### GETTER AND SETTER FUNCTION DEFINITION #########################################
// #########################################################################################################################

void ComInterface::setAnimation(int value){
  animation = value;
}

int ComInterface::getAnimation(){
  return animation;
}

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
      // ###########################################################
      // ################# The Fire animation #######################
      case 4:
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
      case 9:
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
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = START_MARKER;
  char endMarker = END_MARKER;
  char rc;

  while (Serial.available() > 0 && newData == false) {
      rc = Serial.read();

      if (recvInProgress == true) {
          if (rc != endMarker) {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) {
                  ndx = numChars - 1;
              }
          }
          else {
              receivedChars[ndx] = '\0'; // terminate the string
              recvInProgress = false;
              ndx = 0;
              newData = true;
          }
      }

      else if (rc == startMarker) {
          recvInProgress = true;
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

  // The first element in the list
  strtokIndx = strtok(tempChars,",");
  currentArray = atoi(strtokIndx); // The current array being manipulated
  // The second element in the list
  strtokIndx = strtok(NULL, ",");

  animation = atoi(strtokIndx); // The animation to be switched to

  strtokIndx = strtok(NULL, ",");
  // For each subsequent parameter define apropriate type and sets its value in the the appropriate reference table.
  while(strtokIndx != NULL) {
      
    // Finds the type of the current parameter value
    type_indicator = findParamType(animation, param_buffer_count);

    // This switch casts the parameter to the appropriate type and sets the appropriate possition in the appropriat table with that value.
    switch (type_indicator) {
      case 1:
        parameter_value_float = atof(strtokIndx);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parametersFloat, currentArray, animation, param_buffer_count, parameter_value_float);
        Animations::setParametersFloat(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_float);
        break;
      case 2:
        parameter_value_unsigned_long  = (unsigned long)atoi(strtokIndx);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parametersUnsignedLong, currentArray, animation, param_buffer_count, parameter_value_unsigned_long);
        Animations::setParametersUnsignedLong(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_unsigned_long);
        break;
      default:
        parameter_value_int  = atoi(strtokIndx);
        //outputAndReplace(NB_ARRAYS, NB_ANIMATIONS, NB_MAX_PARAMS, parametersInt, currentArray, animation, param_buffer_count, parameter_value_int);
        Animations::setParametersInt(animParamRefs, currentArray, animation, param_buffer_count, parameter_value_int);
    }


      // For each iteration of the loop will tell 
      param_buffer_count++;
      // Leave this at end
      strtokIndx = strtok(NULL, ",");
  }

      /* Original code
      char * strtokIndx; // this is used by strtok() as an index

      strtokIndx = strtok(tempChars,",");      // get the first part - the string
      strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

      strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
      integerFromPC = atoi(strtokIndx);     // convert this part to an integer

      strtokIndx = strtok(NULL, ",");
      floatFromPC = atof(strtokIndx);     // convert this part to a float
      */
}

void ComInterface::readInput(){ 
  ComInterface::recvWithStartEndMarkers();
  if (newData == true) {
      strcpy(tempChars, receivedChars);
          // this temporary copy is necessary to protect the original data
          //   because strtok() used in parseData() replaces the commas with \0
      ComInterface::parseAnimationChangeData();
      newData = false;
  }
}